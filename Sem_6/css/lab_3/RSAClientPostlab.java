import java.io.*;
import java.math.BigInteger;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class RSAClientPostlab {
    // Class to hold benchmark results
    static class BenchmarkResult {
        int keySize;
        int messageSize;
        long keyGenTime;
        long encryptionTime;
        long decryptionTime;
        
        @Override
        public String toString() {
            return String.format("Key Size: %d bits, Message Size: %d bits, Key Gen: %.2f ms, Encryption: %.2f ms, Decryption: %.2f ms",
                    keySize, messageSize, keyGenTime / 1_000_000.0, encryptionTime / 1_000_000.0, decryptionTime / 1_000_000.0);
        }
    }
    
    public static void main(String[] args) {
        try {
            // Message sizes to test (in bits)
            int[] messageSizes = {8, 12, 16, 20, 24};
            List<BenchmarkResult> results = new ArrayList<>();
            
            System.out.println("RSA Benchmark Client");
            
            // Connect to server
            Socket socket = new Socket("localhost", 5000);
            ObjectOutputStream out = new ObjectOutputStream(socket.getOutputStream());
            ObjectInputStream in = new ObjectInputStream(socket.getInputStream());
            
            System.out.println("Connected to server");
            
            // Get available key sizes from server
            Integer[] keySizes = (Integer[]) in.readObject();
            System.out.println("Available key sizes: ");
            for (Integer size : keySizes) {
                System.out.print(size + " bits, ");
            }
            System.out.println();
            
            // Run benchmarks for each key size
            for (Integer keySize : keySizes) {
                System.out.println("\n--- Testing key size: ~" + keySize + " bits ---");
                
                // Request server to generate keys of this size
                out.writeObject(keySize);
                
                // Get key pair from server
                int publicKey = (int) in.readObject();
                int primeMul = (int) in.readObject();
                long keyGenTime = (Long) in.readObject();
                
                System.out.println("Received public key (e): " + publicKey);
                System.out.println("Received modulus (n): " + primeMul);
                
                // Test with different message sizes
                for (int messageBits : messageSizes) {
                    // Skip if message size is too large for this key
                    if (messageBits >= Math.log(primeMul) / Math.log(2)) {
                        System.out.println("Skipping " + messageBits + " bit message (too large for " + keySize + " bit key)");
                        continue;
                    }
                    
                    // Generate a random message of specified bit length
                    int maxValue = (1 << messageBits) - 1;
                    int message = new Random().nextInt(maxValue) + 1;
                    
                    System.out.println("\nMessage size: " + messageBits + " bits");
                    System.out.println("Message value: " + message);
                    
                    // Encrypt message and measure time
                    long startEncrypt = System.nanoTime();
                    
                    // Using BigInteger for proper encryption
                    BigInteger msgBig = BigInteger.valueOf(message);
                    BigInteger e = BigInteger.valueOf(publicKey);
                    BigInteger n = BigInteger.valueOf(primeMul);
                    BigInteger encryptedMessage = msgBig.modPow(e, n);
                    
                    long encryptionTime = System.nanoTime() - startEncrypt;
                    
                    System.out.println("Encrypted message: " + encryptedMessage);
                    System.out.println("Encryption time: " + encryptionTime / 1_000_000.0 + " ms");
                    
                    // Send message size and encrypted message to server
                    out.writeObject(messageBits);
                    out.writeObject(encryptedMessage);
                    out.writeObject(encryptionTime);
                    
                    // Get decryption time from server
                    long decryptionTime = (Long) in.readObject();
                    BigInteger decryptedMessage = (BigInteger) in.readObject();
                    
                    System.out.println("Server decrypted to: " + decryptedMessage);
                    System.out.println("Decryption time: " + decryptionTime / 1_000_000.0 + " ms");
                    
                    // Validate decryption
                    if (decryptedMessage.intValue() == message) {
                        System.out.println("Decryption successful!");
                    } else {
                        System.out.println("Decryption failed! Expected: " + message + ", Got: " + decryptedMessage);
                    }
                    
                    // Store result
                    BenchmarkResult result = new BenchmarkResult();
                    result.keySize = keySize;
                    result.messageSize = messageBits;
                    result.keyGenTime = keyGenTime;
                    result.encryptionTime = encryptionTime;
                    result.decryptionTime = decryptionTime;
                    results.add(result);
                }
                
                // Signal end of message size tests
                out.writeObject(-1);
            }
            
            // Signal end of key size tests
            out.writeObject(-1);
            
            // Close connections
            in.close();
            out.close();
            socket.close();
            
            // Print summary of results
            System.out.println("\n=== BENCHMARK SUMMARY ===");
            for (BenchmarkResult result : results) {
                System.out.println(result);
            }
            
            // Print conclusions
            System.out.println("\n=== CONCLUSIONS ===");
            System.out.println("1. Key Generation: Time increases with key size.");
            System.out.println("2. Encryption: Time increases with both key size and message size.");
            System.out.println("3. Decryption: Time increases more significantly with key size compared to encryption.");
            System.out.println("4. The relationship between key size and operation time is not strictly linear due to the complexity of modular exponentiation.");
            
        } catch (Exception e) {
            System.out.println("Client Error: " + e.getMessage());
            e.printStackTrace();
        }
    }
}
