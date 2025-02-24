import java.util.Scanner;

public class Main1 {
  public static void main(String[] args) {
    Scanner sc = new Scanner(System.in);

    // Read the basic salary from the user
    System.out.print("Enter the basic salary: ");
    int basicSalary = sc.nextInt();
    try {
      // Calculate the gross salary
      int grossSalary = calculateGrossSalary(basicSalary);
      System.out.println("Gross salary: " + grossSalary);
    } catch (PayOutOfBoundsException e) {
      System.out.println(e.getMessage());
    }
  }

  public static int calculateGrossSalary(int basicSalary) throws PayOutOfBoundsException {
    // Check if the basic salary is less than 8000
    if (basicSalary < 8000) {
      // Throw a custom exception if the basic salary is less than 8000
      throw new PayOutOfBoundsException("Basic salary cannot be less than 8000");
    }
    Scanner sc = new Scanner(System.in);
    System.out.print("Enter the Dearness Allowance : ");
    int da = sc.nextInt();
    System.out.print("Enter the House Rent Allowance: ");
    int hra = sc.nextInt();
    System.out.print("Enter the Travelling Allowance : ");
    int ta = sc.nextInt();
    System.out.print("Enter the Professional Tax : ");
    int pt = sc.nextInt();
    System.out.print("Enter the TDS(Tax Deducted at source): ");
    int tds = sc.nextInt();
    // Calculate the gross salary using the given formula
    int grossSalary = basicSalary + da + hra + ta - pt - tds;

    return grossSalary;
  }
}

class PayOutOfBoundsException extends Exception {
  public PayOutOfBoundsException(String message) {
    super(message);
  }
}
