import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Worker {

    private final String masterHost;
    private final int masterPort;
    private final int port;

    public Worker(String masterHost, int masterPort, int port) {
        this.masterHost = masterHost;
        this.masterPort = masterPort;
        this.port = port;
    }

    // helper to replace Map.of
    private static Map<String, Object> map(Object... vals) {
        Map<String, Object> m = new HashMap<String, Object>();
        for (int i = 0; i < vals.length; i += 2) {
            m.put((String) vals[i], vals[i + 1]);
        }
        return m;
    }

    public void start() throws Exception {
        register();

        ServerSocket ss = new ServerSocket(port);
        System.out.println("Worker listening on " + port);

        while (true) {
            Socket s = ss.accept();
            new Thread(() -> handle(s)).start();
        }
    }

    private void register() {
        try (Socket s = new Socket(masterHost, masterPort);
             ObjectOutputStream out = new ObjectOutputStream(s.getOutputStream());
             ObjectInputStream in = new ObjectInputStream(s.getInputStream())) {

            String addr = InetAddress.getLocalHost().getHostAddress() + ":" + port;

            out.writeObject(new MRRequest(MRRequest.Type.REGISTER, map("address", addr)));
            out.flush();

            MRResponse r = (MRResponse) in.readObject();
            if (!r.ok) {
                System.err.println("Register failed: " + r.error);
            }

        } catch (Exception e) {
            System.err.println("Register error: " + e.getMessage());
        }
    }

    private void handle(Socket s) {
        try (ObjectInputStream in = new ObjectInputStream(s.getInputStream());
             ObjectOutputStream out = new ObjectOutputStream(s.getOutputStream())) {

            MRRequest req = (MRRequest) in.readObject();

            if (req.type == MRRequest.Type.MAP_TASK) {

                String mapperClass = (String) req.data.get("mapper");
                String splitId = (String) req.data.get("splitId");

                @SuppressWarnings("unchecked")
                List<String> lines = (List<String>) req.data.get("lines");

                List<KV> kvs = doMap(mapperClass, splitId, lines);
                out.writeObject(MRResponse.ok(map("kvs", kvs)));

            } else if (req.type == MRRequest.Type.REDUCE_TASK) {

                String reducerClass = (String) req.data.get("reducer");
                String key = (String) req.data.get("key");

                @SuppressWarnings("unchecked")
                List<String> values = (List<String>) req.data.get("values");

                String res = doReduce(reducerClass, key, values);
                out.writeObject(MRResponse.ok(map("result", res)));

            } else {
                out.writeObject(MRResponse.fail("Unsupported at worker"));
            }

            out.flush();

        } catch (Exception e) {
            System.err.println("Worker handler error: " + e);
        } finally {
            try {
                s.close();
            } catch (IOException ignored) {}
        }
    }

    private List<KV> doMap(String mapperClass, String splitId, List<String> lines) throws Exception {
        Mapper mapper = (Mapper) Class.forName(mapperClass).getDeclaredConstructor().newInstance();
        Collector c = new Collector();
        int lineno = 0;
        for (String line : lines) {
            mapper.map(splitId + ":" + lineno, line, c);
            lineno++;
        }
        return c.get();
    }

    private String doReduce(String reducerClass, String key, List<String> values) throws Exception {
        Reducer reducer = (Reducer) Class.forName(reducerClass).getDeclaredConstructor().newInstance();
        Collector c = new Collector();
        reducer.reduce(key, values, c);

        StringBuilder sb = new StringBuilder();
        for (KV kv : c.get()) {
            if (sb.length() > 0) {
                sb.append(",");
            }
            sb.append(kv.value);
        }
        return sb.toString();
    }

    public static void main(String[] args) throws Exception {
        if (args.length != 3) {
            System.out.println("Usage: java Worker <masterHost> <masterPort> <port>");
            return;
        }
        new Worker(args[0], Integer.parseInt(args[1]), Integer.parseInt(args[2])).start();
    }
}
