import java.io.*;
import java.net.*;
import java.util.*;
import java.util.concurrent.*;

public class MetadataServer {

    private final int port;

    private final Map<String, List<String>> fileToChunks = new ConcurrentHashMap<>();
    private final Map<String, List<String>> chunkToLocations = new ConcurrentHashMap<>();
    private final Set<String> chunkServers = ConcurrentHashMap.newKeySet();

    private final Map<String, Long> lastHeartbeat = new ConcurrentHashMap<>();
    private static final long HEARTBEAT_TIMEOUT_MS = 20000;

    private final int replicationFactor = 2;

    public MetadataServer(int port) {
        this.port = port;
    }

    // ----------------------------
    // Helper for Java 8 (replaces Map.of)
    // ----------------------------
    private static Map<String, Object> map(Object... vals) {
        Map<String, Object> m = new HashMap<>();
        for (int i = 0; i < vals.length; i += 2) {
            m.put((String) vals[i], vals[i + 1]);
        }
        return m;
    }

    public void start() throws Exception {

        Executors.newSingleThreadScheduledExecutor()
                .scheduleAtFixedRate(
                        this::cleanupDeadChunkServers,
                        5, 5, TimeUnit.SECONDS
                );

        ServerSocket ss = new ServerSocket(port);
        System.out.println("MetadataServer listening on " + port);

        while (true) {
            final Socket s = ss.accept();
            Executors.newCachedThreadPool().execute(() -> handleConnection(s));
        }
    }

    private void handleConnection(Socket s) {
        try (
            ObjectInputStream in = new ObjectInputStream(s.getInputStream());
            ObjectOutputStream out = new ObjectOutputStream(s.getOutputStream())
        ) {
            Request req = (Request) in.readObject();
            Response resp = processRequest(req);

            out.writeObject(resp);
            out.flush();

        } catch (Exception e) {
            System.err.println("Metadata connection error: " + e);
        } finally {
            try { s.close(); } catch (Exception ignored) {}
        }
    }

    private synchronized Response processRequest(Request req) {
        try {

            switch (req.type) {

                case REGISTER_CHUNKSERVER: {
                    String addr = (String) req.data.get("address");
                    chunkServers.add(addr);

                    lastHeartbeat.put(addr, System.currentTimeMillis());

                    return Response.success(map("message", "registered"));
                }

                case HEARTBEAT: {
                    String addr = (String) req.data.get("address");
                    lastHeartbeat.put(addr, System.currentTimeMillis());
                    return Response.success(map("message", "heartbeat"));
                }

                case CREATE_FILE: {

                    String filename = (String) req.data.get("filename");
                    long filesize = (Long) req.data.get("size");
                    int chunkSize = (Integer) req.data.get("chunkSize");

                    int numChunks = (int) Math.max(1, Math.ceil(filesize * 1.0 / chunkSize));

                    List<String> chunkIds = new ArrayList<>();
                    List<Map<String, Object>> allocation = new ArrayList<>();

                    List<String> available = new ArrayList<>(chunkServers);
                    if (available.isEmpty())
                        return Response.fail("No chunkservers available");

                    for (int i = 0; i < numChunks; i++) {

                        String chunkId = filename + "_chunk_" + i + "_" + UUID.randomUUID();
                        chunkIds.add(chunkId);

                        List<String> chosen = new ArrayList<>();

                        for (int r = 0; r < replicationFactor && !available.isEmpty(); r++) {
                            String pick = available.get((i + r) % available.size());
                            if (!chosen.contains(pick))
                                chosen.add(pick);
                        }

                        chunkToLocations.put(chunkId, new ArrayList<>(chosen));

                        allocation.add(
                                map("chunkId", chunkId, "locations", chosen)
                        );
                    }

                    fileToChunks.put(filename, chunkIds);

                    return Response.success(map("chunks", allocation));
                }

                case GET_FILE_INFO: {

                    String filename = (String) req.data.get("filename");

                    List<String> chunks = fileToChunks.get(filename);
                    if (chunks == null)
                        return Response.fail("File not found");

                    List<Map<String, Object>> infos = new ArrayList<>();

                    for (String cid : chunks) {
                        List<String> locs = chunkToLocations.get(cid);
                        if (locs == null) locs = new ArrayList<>();

                        infos.add(map("chunkId", cid, "locations", locs));
                    }

                    return Response.success(map("chunks", infos));
                }

                case SHUTDOWN:
                    return Response.success(map("message", "bye"));

                default:
                    return Response.fail("Unsupported request");
            }

        } catch (Exception e) {
            e.printStackTrace();
            return Response.fail("Exception: " + e.getMessage());
        }
    }

    private void cleanupDeadChunkServers() {
        long now = System.currentTimeMillis();
        List<String> dead = new ArrayList<>();

        // Java 8-compatible for-each
        for (Map.Entry<String, Long> entry : lastHeartbeat.entrySet()) {
            if (now - entry.getValue() > HEARTBEAT_TIMEOUT_MS) {
                dead.add(entry.getKey());
            }
        }

        for (String addr : dead) {
            System.out.println("Chunkserver timed out: " + addr);

            chunkServers.remove(addr);
            lastHeartbeat.remove(addr);

            for (Map.Entry<String, List<String>> entry : chunkToLocations.entrySet()) {
                entry.getValue().remove(addr);
            }
        }
    }

    public static void main(String[] args) throws Exception {

        if (args.length < 1) {
            System.out.println("Usage: java MetadataServer <port>");
            return;
        }

        int port = Integer.parseInt(args[0]);
        new MetadataServer(port).start();
    }
}
