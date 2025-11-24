import java.io.*;
import java.net.*;
import java.util.*;

public class Client {

    private final String metadataHost;
    private final int metadataPort;

    private final int chunkSize = 1024 * 1024; // 1MB

    public Client(String metadataHost, int metadataPort) {
        this.metadataHost = metadataHost;
        this.metadataPort = metadataPort;
    }

    private static Map<String,Object> map(Object... vals) {
    Map<String,Object> m = new HashMap<>();
    for (int i = 0; i < vals.length; i += 2) {
        m.put((String) vals[i], vals[i+1]);
    }
    return m;
}


    // -------------------- Upload --------------------
    public void upload(String localPath, String remoteName) throws Exception {

        File f = new File(localPath);
        if (!f.exists()) throw new FileNotFoundException(localPath);

        long size = f.length();

        // ask metadata server
        Response r = sendMetadataRequest(
                new Request(Request.Type.CREATE_FILE,
                        map("filename", remoteName,
                                "size", size,
                                "chunkSize", chunkSize))
        );

        if (!r.ok)
            throw new RuntimeException("Create failed: " + r.error);

        @SuppressWarnings("unchecked")
        List<Map<String, Object>> chunks =
                (List<Map<String, Object>>) r.data.get("chunks");

        try (FileInputStream fis = new FileInputStream(f)) {

            byte[] buffer = new byte[8192];

            for (Map<String, Object> cinfo : chunks) {

                String chunkId = (String) cinfo.get("chunkId");

                @SuppressWarnings("unchecked")
                List<String> locs = (List<String>) cinfo.get("locations");

                ByteArrayOutputStream baos = new ByteArrayOutputStream();
                int remaining = chunkSize;

                while (remaining > 0) {
                    int read = fis.read(buffer, 0, Math.min(buffer.length, remaining));
                    if (read <= 0) break;

                    baos.write(buffer, 0, read);
                    remaining -= read;
                }

                byte[] chunkBytes = baos.toByteArray();

                // replicate to all chunkservers
                for (String loc : locs) {

                    String[] hp = loc.split(":");
                    String host = hp[0];
                    int port = Integer.parseInt(hp[1]);

                    System.out.println("Uploading chunk " + chunkId +
                            " to " + loc + " size=" + chunkBytes.length);

                    try (Socket s = new Socket(host, port);
                         ObjectOutputStream out = new ObjectOutputStream(s.getOutputStream());
                         ObjectInputStream in = new ObjectInputStream(s.getInputStream())) {

                        out.writeObject(
                                new Request(Request.Type.STORE_CHUNK,
                                        map("chunkId", chunkId,
                                                "length", chunkBytes.length))
                        );
                        out.flush();

                        OutputStream os = s.getOutputStream();
                        os.write(chunkBytes);
                        os.flush();

                        Response rr = (Response) in.readObject();
                        if (!rr.ok)
                            System.err.println("Store failed on " + loc + ": " + rr.error);

                    } catch (Exception e) {
                        System.err.println("Upload error to " + loc + ": " + e.getMessage());
                    }
                }
            }
        }

        System.out.println("Upload complete for " + remoteName);
    }

    // -------------------- Download --------------------
    public void download(String remoteName, String outPath) throws Exception {

        Response r = sendMetadataRequest(
                new Request(Request.Type.GET_FILE_INFO,
                        map("filename", remoteName))
        );

        if (!r.ok)
            throw new RuntimeException("Get info failed: " + r.error);

        @SuppressWarnings("unchecked")
        List<Map<String, Object>> chunks =
                (List<Map<String, Object>>) r.data.get("chunks");

        try (FileOutputStream fos = new FileOutputStream(outPath)) {

            for (Map<String, Object> cinfo : chunks) {

                String chunkId = (String) cinfo.get("chunkId");

                @SuppressWarnings("unchecked")
                List<String> locs = (List<String>) cinfo.get("locations");

                boolean fetched = false;

                for (String loc : locs) {
                    try {
                        String[] hp = loc.split(":");
                        String host = hp[0];
                        int port = Integer.parseInt(hp[1]);

                        try (Socket s = new Socket(host, port);
                             ObjectOutputStream out = new ObjectOutputStream(s.getOutputStream());
                             ObjectInputStream in = new ObjectInputStream(s.getInputStream())) {

                            out.writeObject(
                                    new Request(Request.Type.FETCH_CHUNK,
                                            map("chunkId", chunkId))
                            );
                            out.flush();

                            Response rr = (Response) in.readObject();
                            if (!rr.ok) {
                                System.err.println("fetch failed: " + rr.error);
                                continue;
                            }

                            int len = ((Long) rr.data.get("length")).intValue();
                            InputStream is = s.getInputStream();

                            byte[] buf = new byte[8192];
                            int remaining = len;

                            while (remaining > 0) {
                                int read = is.read(buf, 0, Math.min(buf.length, remaining));
                                if (read == -1) break;

                                fos.write(buf, 0, read);
                                remaining -= read;
                            }

                            fetched = true;
                            break;
                        }

                    } catch (Exception e) {
                        System.err.println("Attempt failed for " + loc + ": " + e.getMessage());
                    }
                }

                if (!fetched)
                    throw new RuntimeException("Failed to fetch chunk " + chunkId);
            }
        }

        System.out.println("Downloaded file to " + outPath);
    }

    // -------------------- Send request to Metadata Server --------------------
    private Response sendMetadataRequest(Request req) throws Exception {

        try (Socket s = new Socket(metadataHost, metadataPort);
             ObjectOutputStream out = new ObjectOutputStream(s.getOutputStream());
             ObjectInputStream in = new ObjectInputStream(s.getInputStream())) {

            out.writeObject(req);
            out.flush();

            return (Response) in.readObject();
        }
    }

    // -------------------- MAIN --------------------
    public static void main(String[] args) throws Exception {

        if (args.length < 1) {
            System.out.println("Usage: java Client <upload|download> <metadataHost> <metadataPort> ...");
            return;
        }

        String op = args[0];

        if (op.equals("upload")) {

            if (args.length != 5) {
                System.out.println("Usage: java Client upload <metadataHost> <metadataPort> <localPath> <remoteName>");
                return;
            }

            String mh = args[1];
            int mp = Integer.parseInt(args[2]);
            String localPath = args[3];
            String remote = args[4];

            new Client(mh, mp).upload(localPath, remote);

        } else if (op.equals("download")) {

            if (args.length != 5) {
                System.out.println("Usage: java Client download <metadataHost> <metadataPort> <remoteName> <outPath>");
                return;
            }

            String mh = args[1];
            int mp = Integer.parseInt(args[2]);
            String remote = args[3];
            String outPath = args[4];

            new Client(mh, mp).download(remote, outPath);

        } else {
            System.out.println("Unknown operation: " + op);
        }
    }
}
