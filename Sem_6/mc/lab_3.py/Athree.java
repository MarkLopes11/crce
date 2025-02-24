import java.util.Random;
import java.math.BigInteger;

public class Athree {
    public static void main(String[] args) {
        Random random = new Random();

        // Generate two random 64-bit values
        long k = random.nextLong();
        long m = random.nextLong();

        // Convert to binary strings (ensuring full 64-bit representation)
        String kb = String.format("%64s", Long.toBinaryString(k)).replace(' ', '0');
        String mb = String.format("%64s", Long.toBinaryString(m)).replace(' ', '0');

        // Construct a 128-bit binary string by padding with zeros
        String fullKb = String.format("%64s", kb).replace(' ', '0') + String.format("%64s", kb).replace(' ', '0');
        String fullMb = String.format("%64s", mb).replace(' ', '0') + String.format("%64s", mb).replace(' ', '0');

        System.out.println("k (128-bit binary): " + fullKb);
        System.out.println("m (128-bit binary): " + fullMb);

        // Split the binary strings into left and right halves
        String kbl = fullKb.substring(0, 64);
        String kbr = fullKb.substring(64);
        String mbl = fullMb.substring(0, 64);
        String mbr = fullMb.substring(64);

        // Convert left and right halves to BigInteger for safe binary operations
        BigInteger k1 = new BigInteger(kbl, 2);
        BigInteger k2 = new BigInteger(kbr, 2);
        BigInteger m1 = new BigInteger(mbl, 2);
        BigInteger m2 = new BigInteger(mbr, 2);

        // XOR operation for a3
        BigInteger result1 = k1.xor(m1);
        BigInteger result2 = k2.xor(m2);
        BigInteger a3 = result1.xor(result2);

        // Print the result of a3 in binary format
        System.out.println("SRES = " + a3.toString(2));
    }
}