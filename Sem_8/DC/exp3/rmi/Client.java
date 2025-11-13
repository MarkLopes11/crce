import java.rmi.Naming;
import java.util.Scanner;

public class Client {
    public static void main(String[] args) {
        try {
            Calculator calc = (Calculator) Naming.lookup("rmi://localhost/calculator");
            System.out.println("Connected to Calculator Service");

            Scanner sc = new Scanner(System.in);
            System.out.print("Enter first number: ");
            int a = sc.nextInt();
            System.out.print("Enter second number: ");
            int b = sc.nextInt();

            System.out.println("Add: " + calc.add(a, b));
            System.out.println("Sub: " + calc.sub(a, b));
            System.out.println("Mul: " + calc.mul(a, b));
            System.out.println("Div: " + calc.div(a, b));

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
