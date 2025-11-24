import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.HashMap;
import java.util.Map;

public class JobClient {

    public static void main(String[] args) throws Exception {

        if (args.length != 7) {
            System.out.println("Usage: java JobClient <masterHost> <masterPort> <inputPath> <mapperClass> <reducerClass> <splits> <outPath>");
            return;
        }

        String mh = args[0];
        int mp = Integer.parseInt(args[1]);
        String input = args[2];
        String mapper = args[3];
        String reducer = args[4];
        int splits = Integer.parseInt(args[5]);
        String out = args[6];

        try (Socket s = new Socket(mh, mp);
             ObjectOutputStream outo = new ObjectOutputStream(s.getOutputStream());
             ObjectInputStream ino = new ObjectInputStream(s.getInputStream())) {

            Map<String, Object> data = new HashMap<String, Object>();
            data.put("inputPath", input);
            data.put("mapper", mapper);
            data.put("reducer", reducer);
            data.put("splits", splits);
            data.put("outPath", out);

            outo.writeObject(new MRRequest(MRRequest.Type.SUBMIT, data));
            outo.flush();

            MRResponse resp = (MRResponse) ino.readObject();
            if (resp.ok) {
                System.out.println("Job finished: " + resp.data);
            } else {
                System.err.println("Job failed: " + resp.error);
            }
        }
    }
}
