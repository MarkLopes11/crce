import java.rmi.Naming;
import java.rmi.registry.LocateRegistry;

public class Server {
    public static void main(String[] args) {
        try {
            LocateRegistry.createRegistry(1099);
            System.out.println("RMI Registry Started...");

            CalculatorImpl calc = new CalculatorImpl();

            Naming.rebind("rmi://localhost/calculator", calc);

            System.out.println("Calculator Service is Ready...");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
