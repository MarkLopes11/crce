class MyChildThread extends Thread {
    public void run() {
        System.out.println("Child Thread is now active");
        System.out.println("Child Thread's Priority: " + getPriority());
        System.out.println("Child Thread's Name: " + getName());
        Thread.yield(); // Yield to another thread
        System.out.println("Child Thread resumed after yielding");
    }
}

public class ThreadMethodsDemo {
    public static void main(String[] args) {
        // Create a custom child thread
        MyChildThread childThread = new MyChildThread();

        // Set thread priority and name for the child thread
        childThread.setPriority(8);
        childThread.setName("CustomChildThread");

        // Launch the child thread
        childThread.start();

        // Main thread's journey begins
        Thread mainThread = Thread.currentThread();
        System.out.println("Main Thread is in action");
        System.out.println("Main Thread's Priority: " + mainThread.getPriority());
        System.out.println("Main Thread's Name: " + mainThread.getName());

        try {
            // Main thread sleeps for 2 seconds
            System.out.println("Main Thread is going to sleep for 2 seconds");
            Thread.sleep(2000);

            // Main thread joins the child thread
            childThread.join();
            System.out.println("Main Thread is awake after joining the child thread");
        } catch (InterruptedException e) {
            System.out.println("Main Thread was interrupted.");
        }

        // Main thread's adventure continues
        System.out.println("Main Thread is now complete");

        
    }
}
