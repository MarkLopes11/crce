class BankAccount {
    private double balance;

    public BankAccount(double initialBalance) {
        // Initialize the bank account with an initial balance.
        balance = initialBalance;
    }

    public synchronized void deposit(double amount) {
        // Deposit money into the account and show the new balance.
        balance += amount;
        System.out.println("Deposited: " + amount + " | New Balance: " + balance);
    }

    public synchronized void withdraw(double amount) {
        if (balance >= amount) {
            // If there's enough balance, withdraw money and show the new balance.
            balance -= amount;
            System.out.println("Withdrawn: " + amount + " | New Balance: " + balance);
        } else {
            // If there's not enough balance, show a message.
            System.out.println("Insufficient balance to withdraw " + amount);
        }
    }
}

class DepositThread extends Thread {
    private BankAccount account;
    private double amount;

    public DepositThread(BankAccount account, double amount) {
        this.account = account;
        this.amount = amount;
    }

    public void run() {
        // Perform a deposit transaction.
        account.deposit(amount);
    }
}

class WithdrawThread extends Thread {
    private BankAccount account;
    private double amount;

    public WithdrawThread(BankAccount account, double amount) {
        this.account = account;
        this.amount = amount;
    }

    public void run() {
        // Perform a withdrawal transaction.
        account.withdraw(amount);
    }
}

public class BankTransactionDemo {
    public static void main(String[] args) {
        // Create a bank account with an initial balance of $1000.
        BankAccount account = new BankAccount(1000.0);

        // Create threads to simulate transactions.
        DepositThread depositThread1 = new DepositThread(account, 200.0);
        WithdrawThread withdrawThread1 = new WithdrawThread(account, 300.0);
        DepositThread depositThread2 = new DepositThread(account, 500.0);
        WithdrawThread withdrawThread2 = new WithdrawThread(account, 700.0);

        // Start the threads to perform transactions.
        depositThread1.start();
        withdrawThread1.start();
        depositThread2.start();
        withdrawThread2.start();
    }
}
