import java.util.Scanner;

public class DecimalToHexadecimal {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter a decimal number: ");
        int decimalNumber = scanner.nextInt();

        String hexadecimal = decimalToHexadecimal(decimalNumber);

        System.out.println("The hexadecimal representation is " + hexadecimal);
    }

    static String decimalToHexadecimal(int decimalNumber) {
        String hexadecimal = Integer.toHexString(decimalNumber).toUpperCase();
        return hexadecimal;
    }
}
