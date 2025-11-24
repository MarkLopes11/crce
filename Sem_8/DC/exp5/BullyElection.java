import java.util.*;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicBoolean;

public class BullyElection {
    public static void main(String[] args) throws Exception {
        final int N = 6; // number of nodes (IDs 1..N)
        final List<BlockingQueue<Message>> queues = new ArrayList<>();

        for (int i = 0; i < N + 1; i++)
            queues.add(new LinkedBlockingQueue<>()); // index by id

        List<Node> nodes = new ArrayList<>();

        for (int id = 1; id <= N; id++) {
            Node node = new Node(id, N, queues);
            nodes.add(node);
            new Thread(node, "Node-" + id).start();
        }

        Thread.sleep(2000);

        System.out.println("\n--- Simulate coordinator failure ---\n");

        // fail the highest ID
        int failId = N;
        for (Node node : nodes) {
            if (node.id == failId) {
                node.fail();
                break;
            }
        }

        Thread.sleep(8000);

        System.out.println("\n--- Recovering failed node ---\n");

        for (Node node : nodes) {
            if (node.id == failId) {
                node.recover();
                break;
            }
        }

        Thread.sleep(8000);

        for (Node node : nodes) node.shutdown();

        System.out.println("\nSimulation finished.");
    }
}

enum MsgType { ELECTION, OK, COORDINATOR, HEARTBEAT }

class Message {
    final MsgType type;
    final int senderId;
    final int coordinatorId;

    Message(MsgType type, int senderId) {
        this(type, senderId, -1);
    }

    Message(MsgType type, int senderId, int coordinatorId) {
        this.type = type;
        this.senderId = senderId;
        this.coordinatorId = coordinatorId;
    }

    public String toString() {
        if (type == MsgType.COORDINATOR) {
            return type + "(from " + senderId + ", coord=" + coordinatorId + ")";
        } else {
            return type + "(from " + senderId + ")";
        }
    }
}

class Node implements Runnable {
    final int id;
    final int N;
    final List<BlockingQueue<Message>> queues;
    final BlockingQueue<Message> myQueue;
    volatile int currentCoordinator = -1;

    final AtomicBoolean alive = new AtomicBoolean(true);
    final AtomicBoolean running = new AtomicBoolean(true);

    final Random rnd = new Random();
    volatile long lastHeartbeat = System.currentTimeMillis();

    Node(int id, int N, List<BlockingQueue<Message>> queues) {
        this.id = id;
        this.N = N;
        this.queues = queues;
        this.myQueue = queues.get(id);

        if (id == N) currentCoordinator = N;
    }

    void send(int toId, Message m) {
        try {
            queues.get(toId).put(m);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }

    void broadcast(Message m) {
        for (int i = 1; i <= N; i++) {
            if (i != id) send(i, m);
        }
    }

    void startElection() {
        if (!alive.get()) return;

        System.out.println(logPrefix() + "Starting election");

        boolean higherExists = false;

        for (int j = id + 1; j <= N; j++) {
            higherExists = true;
            send(j, new Message(MsgType.ELECTION, id));
        }

        if (!higherExists) {
            becomeCoordinator();
            return;
        }

        long waitUntil = System.currentTimeMillis() + 1000;
        boolean gotOk = false;

        while (System.currentTimeMillis() < waitUntil) {
            Message m = myQueue.poll();

            if (m == null) {
                try {
                    Thread.sleep(50);
                } catch (InterruptedException ignored) {}
                continue;
            }

            if (m.type == MsgType.OK) {
                gotOk = true;
                break;
            }
        }

        if (!gotOk) {
            becomeCoordinator();
        }
    }

    void becomeCoordinator() {
        currentCoordinator = id;
        System.out.println(logPrefix() + "BECAME COORDINATOR");
        broadcast(new Message(MsgType.COORDINATOR, id, id));
    }

    void fail() {
        System.out.println(logPrefix() + " FAILING (crashed)");
        alive.set(false);
    }

    void recover() {
        System.out.println(logPrefix() + " RECOVERED");
        alive.set(true);
        startElection();
    }

    void shutdown() {
        running.set(false);
    }

    String logPrefix() {
        return "[Node " + id + "] ";
    }

    @Override
    public void run() {
        while (running.get()) {
            if (!alive.get()) {
                try { Thread.sleep(100); } catch (InterruptedException ignored) {}
                continue;
            }

            Message m = myQueue.poll();
            if (m != null) {
                switch (m.type) {
                    case ELECTION:
                        if (alive.get()) {
                            send(m.senderId, new Message(MsgType.OK, id));
                            startElection();
                        }
                        break;

                    case COORDINATOR:
                        currentCoordinator = m.coordinatorId;
                        System.out.println(logPrefix() + "New coordinator is " + currentCoordinator);
                        break;

                    default:
                        break;
                }
            }

            // heartbeat to coordinator
            if (id != currentCoordinator && currentCoordinator != -1) {
                long now = System.currentTimeMillis();
                if (now - lastHeartbeat > 2000) {
                    System.out.println(logPrefix() + "Coordinator missing. Starting election.");
                    startElection();
                }
            }

            try { Thread.sleep(50); } catch (InterruptedException ignored) {}
        }
    }
}
