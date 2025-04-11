import java.io.*;
import java.math.BigDecimal;
import java.math.BigInteger;
import java.net.Socket;
import java.util.Scanner;

public class RSAClient {
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

    public static void main(String[] args) {
        try {
            Scanner scanner = new Scanner(System.in);
            System.out.println("RSA Encryption Client");
            
            // Connect to server
            Socket socket = new Socket("localhost", 5000);
            ObjectOutputStream out = new ObjectOutputStream(socket.getOutputStream());
            ObjectInputStream in = new ObjectInputStream(socket.getInputStream());
            
            System.out.println("Connected to server");
            
            // Get server's public key
            int publicKey = (int) in.readObject();
            int primeMul = (int) in.readObject();
            
            System.out.println("Received public key (e): " + publicKey);
            System.out.println("Received modulus (n): " + primeMul);
            
            // Get message to encrypt
            System.out.print("Enter a message (number) to encrypt: ");
            int message = scanner.nextInt();
            
            // Encrypt message
            double cipher = (Math.pow(message, publicKey)) % primeMul;
            System.out.println("Encrypted message: " + cipher);
            
            // Send encrypted message to server
            out.writeObject(BigDecimal.valueOf(cipher).toBigInteger());
            
            // Get decrypted message from server (for confirmation)
            BigInteger decryptedMessage = (BigInteger) in.readObject();
            System.out.println("Server decrypted the message to: " + decryptedMessage);
            
            // Cleose all coz best practise
            scanner.close();
            in.close();
            out.close();
            socket.close();
            
        } catch (Exception e) {
            System.out.println("Client Error: " + e.getMessage());
            e.printStackTrace();
        }
    }
}
