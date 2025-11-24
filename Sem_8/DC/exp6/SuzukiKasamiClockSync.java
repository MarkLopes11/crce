import java.util.*;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicInteger;

public class SuzukiKasamiClockSync {
    public static void main(String[] args) throws Exception {
        final int N = 5; // number of sites
        final int simulationSeconds = 20;

        // shared message queues (one per node)
        List<BlockingQueue<Message>> queues = new ArrayList<>();
        for (int i = 0; i < N; i++)
            queues.add(new LinkedBlockingQueue<>());

        // create a time server
        TimeServer timeServer = new TimeServer();
        new Thread(timeServer, "TimeServer").start();

        // initial token is given to node 0
        Token initialToken = new Token(N);
        initialToken.LN[0] = 0;

        // create and start nodes
        List<Node> nodes = new ArrayList<>();
        for (int i = 0; i < N; i++) {
            boolean hasToken = (i == 0);
            Node node = new Node(i, N, queues, hasToken ? initialToken : null, timeServer);
            nodes.add(node);
            new Thread(node, "Node-" + i).start();
        }

        // let simulation run
        Thread.sleep(simulationSeconds * 1000L);

        // stop nodes
        for (Node n : nodes) n.shutdown();
        timeServer.shutdown();

        System.out.println("Simulation finished.");
    }
}

// ========================================================
// Message Class
// ========================================================

class Message {
    enum Type { REQUEST, TOKEN }

    final Type type;
    final int senderId;
    final int sn;
    final Token token;
    final int lamport;

    Message(Type type, int senderId, int sn, Token token, int lamport) {
        this.type = type;
        this.senderId = senderId;
        this.sn = sn;
        this.token = token;
        this.lamport = lamport;
    }

    static Message request(int from, int sn, int lamport) {
        return new Message(Type.REQUEST, from, sn, null, lamport);
    }

    static Message token(int from, Token token, int lamport) {
        return new Message(Type.TOKEN, from, -1, token, lamport);
    }
}

// ========================================================
// Token Class
// ========================================================

class Token {
    int[] LN;
    Deque<Integer> Q;

    Token(int n) {
        LN = new int[n];
        Q = new ArrayDeque<>();
        Arrays.fill(LN, 0);
    }

    Token deepCopy() {
        Token t = new Token(LN.length);
        System.arraycopy(this.LN, 0, t.LN, 0, LN.length);
        t.Q.addAll(this.Q);
        return t;
    }
}

// ========================================================
// Time Server (Cristian Algorithm)
// ========================================================

class TimeServer implements Runnable {
    private volatile boolean running = true;

    long now() { return System.currentTimeMillis(); }

    public void shutdown() { running = false; }

    public void run() {
        while (running) {
            try { Thread.sleep(1000); }
            catch (InterruptedException e) { break; }
        }
    }
}

// ========================================================
// Node Implementation (Suzuki–Kasami + Lamport + Cristian)
// ========================================================

class Node implements Runnable {
    final int id, N;
    final List<BlockingQueue<Message>> queues;
    final BlockingQueue<Message> myQueue;
    final TimeServer timeServer;

    volatile boolean running = true;

    int[] RN;
    Token token;
    boolean hasToken = false;

    final AtomicInteger mySeq = new AtomicInteger(0);
    int lamport = 0;

    final Random rnd = new Random();

    Node(int id, int N, List<BlockingQueue<Message>> queues, Token initialToken, TimeServer timeServer) {
        this.id = id;
        this.N = N;
        this.queues = queues;
        this.myQueue = queues.get(id);
        this.timeServer = timeServer;

        this.RN = new int[N];
        Arrays.fill(RN, 0);

        if (initialToken != null) {
            this.token = initialToken;
            this.hasToken = true;
        }
    }

    // ----------------------------------------------------
    // Request Critical Section
    // ----------------------------------------------------
    void requestCS() {
        lamport++;

        int sn = mySeq.incrementAndGet();
        RN[id] = sn;

        System.out.println(timestamp() + "Node " + id + " requests CS with sn=" + sn +
                " (lamport=" + lamport + ")");

        if (!hasToken) {
            for (int j = 0; j < N; j++) {
                if (j == id) continue;
                sendMessage(j, Message.request(id, sn, lamport));
            }
        }

        // wait until token arrives
        while (running && !hasToken) {
            try { Thread.sleep(20); }
            catch (InterruptedException e) { break; }
        }

        if (hasToken) {
            enterCS();
            releaseCS();
        }
    }

    void enterCS() {
        lamport++;
        System.out.println(timestamp() + "*** Node " + id + " ENTERING CS (lamport=" +
                lamport + ") ***");

        try { Thread.sleep(200 + rnd.nextInt(300)); }
        catch (InterruptedException ignored) {}

        System.out.println(timestamp() + "*** Node " + id + " EXITING CS ***");
    }

    // ----------------------------------------------------
    // Release Critical Section
    // ----------------------------------------------------
    void releaseCS() {
        for (int j = 0; j < N; j++) {
            if (j == id) continue;

            boolean inQueue = token.Q.contains(j);

            if (!inQueue && RN[j] == token.LN[j] + 1)
                token.Q.addLast(j);
        }

        if (!token.Q.isEmpty()) {
            int dest = token.Q.pollFirst();
            Token sendTok = token.deepCopy();
            hasToken = false;
            token = null;

            System.out.println(timestamp() +
                    "Node " + id + " sends token to " + dest);

            sendMessage(dest, Message.token(id, sendTok, lamport));
        } else {
            System.out.println(timestamp() +
                    "Node " + id + " keeps token (Q empty)");
        }
    }

    // ----------------------------------------------------
    // Handle Incoming Messages
    // ----------------------------------------------------
    void handleRequest(Message m) {
        int i = m.senderId;
        RN[i] = Math.max(RN[i], m.sn);
        lamport = Math.max(lamport, m.lamport) + 1;

        if (hasToken && RN[i] == token.LN[i] + 1) {
            Token sendTok = token.deepCopy();
            hasToken = false;
            token = null;

            System.out.println(timestamp() +
                    "Node " + id + " forwards token to " + i);

            sendMessage(i, Message.token(id, sendTok, lamport));
        }
    }

    void handleToken(Message m) {
        this.token = m.token;
        this.hasToken = true;

        lamport = Math.max(lamport, m.lamport) + 1;

        System.out.println(timestamp() +
                "Node " + id + " received token (lamport=" + lamport + ")");
    }

    // ----------------------------------------------------
    // Clock Sync: Cristian Algorithm
    // ----------------------------------------------------
    void syncClockWithServer() {
        long t0 = System.currentTimeMillis();
        long serverTime = timeServer.now();
        long t1 = System.currentTimeMillis();

        long rtt = t1 - t0;
        long estimated = serverTime + rtt / 2;

        System.out.println(timestamp() +
                "Node " + id + " sync: local=" + System.currentTimeMillis() +
                " server=" + serverTime + " rtt=" + rtt +
                " estimated=" + estimated);
    }

    // ----------------------------------------------------
    // Send Message (with delay simulation)
    // ----------------------------------------------------
    void sendMessage(int to, Message m) {
        new Thread(() -> {
            try { Thread.sleep(10 + rnd.nextInt(50)); }
            catch (InterruptedException ignored) {}

            try { queues.get(to).put(m); }
            catch (InterruptedException ignored) {}
        }).start();
    }

    String timestamp() {
        return "[t=" + System.currentTimeMillis() + "] ";
    }

    // ----------------------------------------------------
    // Node Main Loop
    // ----------------------------------------------------
    public void run() {
        // message processing thread
        Thread msgProc = new Thread(() -> {
            while (running) {
                try {
                    Message m = myQueue.poll(200, TimeUnit.MILLISECONDS);
                    if (m == null) continue;

                    switch (m.type) {
                        case REQUEST: handleRequest(m); break;
                        case TOKEN:   handleToken(m); break;
                    }
                } catch (InterruptedException e) { break; }
            }
        });

        msgProc.start();

        // main behavior
        try {
            while (running) {
                if (rnd.nextDouble() < 0.15) requestCS();
                if (rnd.nextDouble() < 0.07) syncClockWithServer();

                Thread.sleep(200 + rnd.nextInt(300));
            }
        } catch (InterruptedException ignored) {}

        msgProc.interrupt();
    }

    public void shutdown() { running = false; }
}
