import java.util.Scanner;

public class NumberOrder {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        
        System.out.print("Input first number: ");
        int firstNumber = scanner.nextInt();
        
        System.out.print("Input second number: ");
        int secondNumber = scanner.nextInt();
        
        System.out.print("Input third number: ");
        int thirdNumber = scanner.nextInt();
        
        if (firstNumber < secondNumber && secondNumber < thirdNumber) {
            System.out.println("Increasing");
        } else if (firstNumber > secondNumber && secondNumber > thirdNumber) {
            System.out.println("Decreasing");
        } else {
            System.out.println("Neither increasing nor decreasing order");
        }
        
    }
}
