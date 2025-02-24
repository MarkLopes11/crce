class PrimeThread extends Thread {
    public void run() {
        long startTime = System.currentTimeMillis();
        int count = 0;
        boolean flag = false;

        System.out.println("Prime Numbers:");
        System.out.println("2");
        try {
            Thread.sleep(200);
        } catch (InterruptedException e) {
        }

        System.out.println("3");
        try {
            Thread.sleep(200);
        } catch (InterruptedException e) {
        }

        for (int n = 2; n < 500; n++) {
            for (int i = 2; i <= n / 2; i++) {
                if (n % i == 0) {
                    flag = false;
                    break;
                } else {
                    flag = true;
                }
            }
            if (flag) {
                System.out.println(n);
                count++;
            }
            if (count == 18) {
                break;
            }
            try {
                Thread.sleep(200);
            } catch (InterruptedException e) {
            }
        }

        long endTime = System.currentTimeMillis();
        System.out.println("Prime Thread execution time: " + (endTime - startTime) + "ms");
    }
}

class FibonacciThread extends Thread {
    public void run() {
        long startTime = System.currentTimeMillis();
        System.out.println("Fibonacci:");
        System.out.println("0");
        try {
            Thread.sleep(200);
        } catch (InterruptedException e) {
        }

        int n = 0, m = 1, s;
        for (int i = 0; i < 14; i++) {
            s = n + m;
            System.out.println(s);
            n = m;
            m = s;
            try {
                Thread.sleep(200);
            } catch (InterruptedException e) {
            }
        }

        long endTime = System.currentTimeMillis();
        System.out.println("Fibonacci Thread execution time: " + (endTime - startTime) + "ms");
    }
}

public class Q3 {
    public static void main(String[] args) {
        PrimeThread primeThread = new PrimeThread();
        FibonacciThread fibonacciThread = new FibonacciThread();

        primeThread.start();
        try {
            primeThread.join();
        } catch (InterruptedException e) {
        }
        fibonacciThread.start();
    }
}
