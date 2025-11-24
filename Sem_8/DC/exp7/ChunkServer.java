import java.io.*;
import java.net.*;
import java.util.*;
import java.util.concurrent.*;

public class ChunkServer {

    private final String metadataHost;
    private final int metadataPort;
    private final int port;

    private final File storageDir;
    private final String address; // host:port

    private final ScheduledExecutorService scheduler =
            Executors.newScheduledThreadPool(1);

    public ChunkServer(String metadataHost, int metadataPort,
                       int port, String storageDir) {

        this.metadataHost = metadataHost;
        this.metadataPort = metadataPort;
        this.port = port;

        this.storageDir = new File(storageDir);
        if (!this.storageDir.exists()) this.storageDir.mkdirs();

        this.address = "localhost:" + port;
    }

    private static Map<String,Object> map(Object... vals) {
    Map<String,Object> m = new HashMap<>();
    for (int i = 0; i < vals.length; i += 2) {
        m.put((String) vals[i], vals[i+1]);
    }
    return m;
}

    public void start() throws Exception {
        registerWithMetadata();

        scheduler.scheduleAtFixedRate(
                this::sendHeartbeat,
                5, 5, TimeUnit.SECONDS
        );

        ServerSocket ss = new ServerSocket(port);
        System.out.println("ChunkServer listening on " + port +
                ", storage = " + storageDir.getAbsolutePath());

        while (true) {
            Socket s = ss.accept();
            Executors.newCachedThreadPool().execute(() -> handleConn(s));
        }
    }

    /** Register ChunkServer with MetadataServer */
    private void registerWithMetadata() {
        try (Socket s = new Socket(metadataHost, metadataPort);
             ObjectOutputStream out = new ObjectOutputStream(s.getOutputStream());
             ObjectInputStream in = new ObjectInputStream(s.getInputStream())) {

            out.writeObject(
                    new Request(Request.Type.REGISTER_CHUNKSERVER,
                            map("address", address))
            );
            out.flush();

            Response r = (Response) in.readObject();
            if (!r.ok)
                System.err.println("Register failed: " + r.error);

        } catch (Exception e) {
            System.err.println("Register exception: " + e.getMessage());
        }
    }

    /** Send heartbeat to metadata server */
    private void sendHeartbeat() {
        try (Socket s = new Socket(metadataHost, metadataPort);
             ObjectOutputStream out = new ObjectOutputStream(s.getOutputStream());
             ObjectInputStream in = new ObjectInputStream(s.getInputStream())) {

            out.writeObject(new Request(
                    Request.Type.HEARTBEAT,
                    map("address", address)
            ));
            out.flush();

            // ignore response
            in.readObject();

        } catch (Exception e) {
            System.err.println("Heartbeat failed: " + e.getMessage());
        }
    }

    /** Handle a new incoming connection */
    private void handleConn(Socket s) {
        try (ObjectInputStream in = new ObjectInputStream(s.getInputStream());
             ObjectOutputStream out = new ObjectOutputStream(s.getOutputStream())) {

            Request req = (Request) in.readObject();

            switch (req.type) {

                case STORE_CHUNK: {
                    String chunkId = (String) req.data.get("chunkId");
                    int len = (Integer) req.data.get("length");

                    File f = new File(storageDir, chunkId);

                    try (FileOutputStream fos = new FileOutputStream(f)) {
                        byte[] buf = new byte[8192];
                        int remaining = len;

                        InputStream is = s.getInputStream();
                        while (remaining > 0) {
                            int toRead = Math.min(buf.length, remaining);
                            int read = is.read(buf, 0, toRead);
                            if (read == -1) break;

                            fos.write(buf, 0, read);
                            remaining -= read;
                        }
                    }

                    out.writeObject(Response.success(map("message", "stored")));
                    out.flush();
                    break;
                }

                case FETCH_CHUNK: {
                    String chunkId = (String) req.data.get("chunkId");
                    File f = new File(storageDir, chunkId);

                    if (!f.exists()) {
                        out.writeObject(Response.fail("chunk not found"));
                        break;
                    }

                    long len = f.length();
                    out.writeObject(Response.success(map("length", len)));
                    out.flush();

                    // send bytes
                    try (FileInputStream fis = new FileInputStream(f);
                         OutputStream os = s.getOutputStream()) {

                        byte[] buf = new byte[8192];
                        int r;

                        while ((r = fis.read(buf)) != -1)
                            os.write(buf, 0, r);

                        os.flush();
                    }
                    break;
                }

                case DELETE_CHUNK: {
                    String chunkId = (String) req.data.get("chunkId");
                    File f = new File(storageDir, chunkId);

                    boolean ok = f.delete();
                    out.writeObject(ok
                            ? Response.success(map("deleted", true))
                            : Response.fail("delete failed"));
                    break;
                }

                default:
                    out.writeObject(Response.fail("unsupported op"));
            }

        } catch (Exception e) {
            System.err.println("ChunkServer conn error: " + e.getMessage());
        } finally {
            try { s.close(); } catch (Exception ignored) {}
        }
    }

    /** Main method */
    public static void main(String[] args) throws Exception {

        if (args.length < 4) {
            System.out.println("Usage: java ChunkServer <metadataHost> <metadataPort> <port> <storageDir>");
            return;
        }

        String mh = args[0];
        int mp = Integer.parseInt(args[1]);
        int port = Integer.parseInt(args[2]);
        String dir = args[3];

        new ChunkServer(mh, mp, port, dir).start();
    }
}
