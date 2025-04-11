import java.io.*;
import java.math.BigInteger;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashMap;
import java.util.Map;

public class RSAServerPostlab {
    // Map to store various prime pairs for different key sizes
    private static final Map<Integer, int[]> PRIME_PAIRS = new HashMap<>();
    
    static {
        // Small key sizes (for demonstration)
        PRIME_PAIRS.put(16, new int[]{53, 59});       // ~16 bits (3127)
        PRIME_PAIRS.put(20, new int[]{389, 103});     // ~20 bits (40067)
        PRIME_PAIRS.put(24, new int[]{1223, 1217});   // ~24 bits (1488391)
        PRIME_PAIRS.put(30, new int[]{12853, 6373});  // ~30 bits (81932969)
    }

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
            System.out.println("RSA Benchmark Server");
            System.out.println("Waiting for client connection...");
            
            Socket clientSocket = serverSocket.accept();
            System.out.println("Client connected: " + clientSocket.getInetAddress());
            
            ObjectOutputStream out = new ObjectOutputStream(clientSocket.getOutputStream());
            ObjectInputStream in = new ObjectInputStream(clientSocket.getInputStream());
            
            // Send available key sizes to client
            out.writeObject(PRIME_PAIRS.keySet().toArray(new Integer[0]));
            
            // Perform benchmarks for different key sizes
            while (true) {
                // Receive key size request from client
                Integer keySize = (Integer) in.readObject();
                if (keySize == -1) break; // Exit signal
                
                int[] primes = PRIME_PAIRS.get(keySize);
                
                // Benchmark key generation
                long startKeyGen = System.nanoTime();
                
                int prime1 = primes[0];
                int prime2 = primes[1];
                int primeMul = prime1 * prime2;
                int phi = (prime1 - 1) * (prime2 - 1);
                int publicKey = findPublicKey(phi);
                int privateKey = calculatePrivateKey(publicKey, phi);
                
                long keyGenTime = System.nanoTime() - startKeyGen;
                
                System.out.println("\nKey Size: ~" + keySize + " bits (n = " + primeMul + ")");
                System.out.println("Using primes: " + prime1 + " and " + prime2);
                System.out.println("Public key (e): " + publicKey);
                System.out.println("Private key (d): " + privateKey);
                System.out.println("Key generation time: " + keyGenTime / 1_000_000.0 + " ms");
                
                // Send key pair to client
                out.writeObject(publicKey);
                out.writeObject(primeMul);
                out.writeObject(keyGenTime);
                
                // Process message size benchmarks
                while (true) {
                    // Get message size from client
                    Integer messageSize = (Integer) in.readObject();
                    if (messageSize == -1) break; // Move to next key size
                    
                    // Receive encrypted message
                    BigInteger encryptedMessage = (BigInteger) in.readObject();
                    Long encryptionTime = (Long) in.readObject();
                    
                    System.out.println("\nMessage size: " + messageSize + " bits");
                    System.out.println("Encryption time (client): " + encryptionTime / 1_000_000.0 + " ms");
                    
                    // Decrypt message and measure time
                    long startDecrypt = System.nanoTime();
                    BigInteger bigN = BigInteger.valueOf(primeMul);
                    BigInteger decryptedMessage = encryptedMessage.modPow(BigInteger.valueOf(privateKey), bigN);
                    long decryptionTime = System.nanoTime() - startDecrypt;
                    
                    System.out.println("Decryption time: " + decryptionTime / 1_000_000.0 + " ms");
                    
                    // Send decryption time back to client
                    out.writeObject(decryptionTime);
                    out.writeObject(decryptedMessage);
                }
            }
            
            // Clean up
            in.close();
            out.close();
            clientSocket.close();
            serverSocket.close();
            System.out.println("Benchmark completed. Server shutdown.");
            
        } catch (Exception e) {
            System.out.println("Server Error: " + e.getMessage());
            e.printStackTrace();
        }
    }
}
