import java.io.*;
import java.math.BigDecimal;
import java.math.BigInteger;
import java.net.ServerSocket;
import java.net.Socket;

public class RSAServer {
    private static int getGCD(int mod, int num) {
        if (mod == 0) {
            return num;
        } else {
            return getGCD(num % mod, mod);
        }
    }

    private static int findPublicKey(int phi) {
        for (int e = 2; e < phi; e++) {
            if (getGCD(e, phi) == 1) {
                return e;
            }
        }
        return -1; // No public key found
    }

    private static int calculatePrivateKey(int e, int phi) {
        for (int k = 0; k <= 9; k++) {
            int temp = 1 + (k * phi);
            if (temp % e == 0) {
                return temp / e;
            }
        }
        return -1; // No private key found
    }

    public static void main(String[] args) {
        try {
            ServerSocket serverSocket = new ServerSocket(5000);
            System.out.println("RSA Decryption Server");
            System.out.println("Waiting for client connection...");
            
            // RSA Key generation
            int prime1 = 53;
            int prime2 = 59;
            int primeMul = prime1 * prime2;
            int phi = (prime1 - 1) * (prime2 - 1);
            
            System.out.println("Using primes: " + prime1 + " and " + prime2);
            System.out.println("n = " + primeMul);
            System.out.println("φ(n) = " + phi);
            
            int publicKey = findPublicKey(phi);
            int privateKey = calculatePrivateKey(publicKey, phi);
            
            System.out.println("Public key (e): " + publicKey);
            System.out.println("Private key (d): " + privateKey);
            
            while (true) {
                Socket clientSocket = serverSocket.accept();
                System.out.println("Client connected: " + clientSocket.getInetAddress());
                
                ObjectOutputStream out = new ObjectOutputStream(clientSocket.getOutputStream());
                ObjectInputStream in = new ObjectInputStream(clientSocket.getInputStream());
                
                // Send public key to client
                out.writeObject(publicKey);
                out.writeObject(primeMul);
                
                // Receive encrypted message
                BigInteger encryptedMessage = (BigInteger) in.readObject();
                System.out.println("Received encrypted message: " + encryptedMessage);
                
                // Decrypt message
                BigInteger bigN = BigInteger.valueOf(primeMul);
                BigInteger decryptedMessage = encryptedMessage.modPow(BigInteger.valueOf(privateKey), bigN);
                
                System.out.println("Decrypted message: " + decryptedMessage);
                
                // Send decrypted message back to client for confirmation
                out.writeObject(decryptedMessage);
                
                // Clean up this connection
                in.close();
                out.close();
                clientSocket.close();
                System.out.println("Client disconnected");
            }
            
        } catch (Exception e) {
            System.out.println("Server Error: " + e.getMessage());
            e.printStackTrace();
        }
    }
}
