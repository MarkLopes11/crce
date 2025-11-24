import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Collections;
import java.util.ConcurrentModificationException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Master {

    private final int port;
    private final List<String> workers = Collections.synchronizedList(new ArrayList<String>());
    private final ExecutorService pool = Executors.newCachedThreadPool();

    public Master(int port) {
        this.port = port;
    }

    // helper to replace Map.of for Java 8
    private static Map<String, Object> map(Object... vals) {
        Map<String, Object> m = new HashMap<String, Object>();
        for (int i = 0; i < vals.length; i += 2) {
            m.put((String) vals[i], vals[i + 1]);
        }
        return m;
    }

    public void start() throws Exception {
        ServerSocket ss = new ServerSocket(port);
        System.out.println("Master listening on " + port);

        while (true) {
            Socket s = ss.accept();
            pool.submit(() -> handle(s));
        }
    }

    private void handle(Socket s) {
        try (ObjectInputStream in = new ObjectInputStream(s.getInputStream());
             ObjectOutputStream out = new ObjectOutputStream(s.getOutputStream())) {

            MRRequest req = (MRRequest) in.readObject();

            if (req == null || req.type == null) {
                out.writeObject(MRResponse.fail("Invalid request or null request type"));
                out.flush();
                return;
            }

            switch (req.type) {

                case REGISTER: {
                    String addr = (String) req.data.get("address");
                    if (!workers.contains(addr)) {
                        workers.add(addr);
                    }
                    System.out.println("Worker registered: " + addr);
                    out.writeObject(MRResponse.ok(map("message", "registered")));
                    break;
                }

                case SUBMIT: {
                    String inputPath = (String) req.data.get("inputPath");
                    String mapperClass = (String) req.data.get("mapper");
                    String reducerClass = (String) req.data.get("reducer");
                    int splits = (Integer) req.data.get("splits");
                    String outPath = (String) req.data.get("outPath");

                    System.out.println("Job received: mapper=" + mapperClass +
                            " reducer=" + reducerClass + " splits=" + splits);

                    try {
                        List<List<String>> splitData = createSplits(inputPath, splits);
                        List<List<KV>> intermediate = assignMapTasks(splitData, mapperClass);
                        Map<String, List<String>> shuffled = shuffle(intermediate);
                        Map<String, String> finalOutput = assignReduceTasks(shuffled, reducerClass);

                        // write output
                        try (BufferedWriter bw = new BufferedWriter(new FileWriter(outPath))) {
                            for (Map.Entry<String, String> e : finalOutput.entrySet()) {
                                bw.write(e.getKey() + "\t" + e.getValue());
                                bw.newLine();
                            }
                        }

                        out.writeObject(MRResponse.ok(map("message", "job finished", "output", outPath)));
                    } catch (Exception ex) {
                        ex.printStackTrace();
                        out.writeObject(MRResponse.fail("job failed: " + ex.getMessage()));
                    }
                    break;
                }

                case MAP_TASK:
                case REDUCE_TASK:
                    out.writeObject(MRResponse.fail("Master doesn't perform map/reduce directly"));
                    break;

                default:
                    out.writeObject(MRResponse.fail("Unsupported request type"));
            }

            out.flush();

        } catch (Exception e) {
            System.err.println("Master handler error: " + e);
        } finally {
            try {
                s.close();
            } catch (Exception ignored) {}
        }
    }

    private List<List<String>> createSplits(String inputPath, int splits) throws IOException {
        List<String> lines = new ArrayList<String>();
        try (BufferedReader br = new BufferedReader(new FileReader(inputPath))) {
            String l;
            while ((l = br.readLine()) != null) {
                lines.add(l);
            }
        }

        List<List<String>> out = new ArrayList<List<String>>();
        for (int i = 0; i < splits; i++) {
            out.add(new ArrayList<String>());
        }

        for (int i = 0; i < lines.size(); i++) {
            out.get(i % splits).add(lines.get(i));
        }

        System.out.println("Created " + splits + " splits");
        return out;
    }

    private List<List<KV>> assignMapTasks(List<List<String>> splitData, String mapperClass) throws Exception {
        final List<List<KV>> results = Collections.synchronizedList(new ArrayList<List<KV>>());
        CountDownLatch latch = new CountDownLatch(splitData.size());

        for (int i = 0; i < splitData.size(); i++) {
            final int idx = i;
            pool.submit(() -> {
                try {
                    String worker = pickWorker(idx);
                    List<KV> res = sendMapTask(worker, mapperClass, "split-" + idx, splitData.get(idx));
                    results.add(res);
                    System.out.println("Map done for split " + idx + " on " + worker +
                            " produced " + res.size() + " kvs");
                } catch (Exception e) {
                    throw new RuntimeException(e);
                } finally {
                    latch.countDown();
                }
            });
        }

        try {
            latch.await();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }

        return results;
    }

    private String pickWorker(int idx) {
        if (workers.isEmpty()) {
            throw new RuntimeException("No workers available");
        }
        return workers.get(idx % workers.size());
    }

    @SuppressWarnings("unchecked")
    private List<KV> sendMapTask(String workerAddr, String mapperClass,
                                 String splitId, List<String> lines) throws Exception {

        String[] hp = workerAddr.split(":");
        try (Socket s = new Socket(hp[0], Integer.parseInt(hp[1]));
             ObjectOutputStream out = new ObjectOutputStream(s.getOutputStream());
             ObjectInputStream in = new ObjectInputStream(s.getInputStream())) {

            Map<String, Object> data = new HashMap<String, Object>();
            data.put("mapper", mapperClass);
            data.put("splitId", splitId);
            data.put("lines", lines);

            out.writeObject(new MRRequest(MRRequest.Type.MAP_TASK, data));
            out.flush();

            MRResponse resp = (MRResponse) in.readObject();
            if (!resp.ok) {
                throw new RuntimeException("Map failed: " + resp.error);
            }
            return (List<KV>) resp.data.get("kvs");
        }
    }

    private Map<String, List<String>> shuffle(List<List<KV>> intermediate) {
        Map<String, List<String>> map = new HashMap<String, List<String>>();

        for (List<KV> list : intermediate) {
            for (KV kv : list) {
                List<String> values = map.get(kv.key);
                if (values == null) {
                    values = new ArrayList<String>();
                    map.put(kv.key, values);
                }
                values.add(kv.value);
            }
        }

        System.out.println("Shuffle produced " + map.size() + " keys");
        return map;
    }

    private Map<String, String> assignReduceTasks(Map<String, List<String>> shuffled,
                                                  String reducerClass) throws Exception {

        Map<String, String> finalMap = new ConcurrentHashMap<String, String>();
        List<String> keys = new ArrayList<String>(shuffled.keySet());
        CountDownLatch latch = new CountDownLatch(keys.size());

        for (int i = 0; i < keys.size(); i++) {
            final String key = keys.get(i);
            final String worker = pickWorker(i);

            pool.submit(() -> {
                try {
                    String outv = sendReduceTask(worker, reducerClass, key, shuffled.get(key));
                    finalMap.put(key, outv);
                    System.out.println("Reduce done for key " + key + " on " + worker);
                } catch (Exception e) {
                    throw new RuntimeException(e);
                } finally {
                    latch.countDown();
                }
            });
        }

        try {
            latch.await();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }

        return finalMap;
    }

    private String sendReduceTask(String workerAddr, String reducerClass,
                                  String key, List<String> values) throws Exception {

        String[] hp = workerAddr.split(":");
        try (Socket s = new Socket(hp[0], Integer.parseInt(hp[1]));
             ObjectOutputStream out = new ObjectOutputStream(s.getOutputStream());
             ObjectInputStream in = new ObjectInputStream(s.getInputStream())) {

            Map<String, Object> data = new HashMap<String, Object>();
            data.put("reducer", reducerClass);
            data.put("key", key);
            data.put("values", values);

            out.writeObject(new MRRequest(MRRequest.Type.REDUCE_TASK, data));
            out.flush();

            MRResponse resp = (MRResponse) in.readObject();
            if (!resp.ok) {
                throw new RuntimeException("Reduce failed: " + resp.error);
            }
            return (String) resp.data.get("result");
        }
    }

    public static void main(String[] args) throws Exception {
        if (args.length != 1) {
            System.out.println("Usage: java Master <port>");
            return;
        }
        int port = Integer.parseInt(args[0]);
        new Master(port).start();
    }
}
