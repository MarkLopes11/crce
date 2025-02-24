class ChildThread extends Thread {
    public void run() {
        System.out.println("Child Thread is now active");
    }
}

public class ThreadExecutionExample {
    public static void main(String[] args) {
        // Create a child thread
        ChildThread childThread = new ChildThread();

        // Launch the child thread
        childThread.start();

        // Main thread's journey begins
        System.out.println("Main Thread is in action");

        // Optionally, wait for the child thread to finish
        try {
            childThread.join();
        } catch (InterruptedException e) {
            System.out.println("Main Thread was interrupted while waiting for the child thread.");
        }

        // Main thread's adventure continues
        System.out.println("Main Thread is now complete");
    }
}
