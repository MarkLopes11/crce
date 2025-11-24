import java.io.*;
import java.net.*;
import java.util.Scanner;

public class RPCClient {

    private final String host;
    private final int port;

    public RPCClient(String host, int port) {
        this.host = host;
        this.port = port;
    }

    public String sendRequest(String request) throws IOException {
        try (
            Socket socket = new Socket(host, port);
            BufferedReader in = new BufferedReader(
                    new InputStreamReader(socket.getInputStream())
            );
            PrintWriter out = new PrintWriter(socket.getOutputStream(), true)
        ) {
            out.println(request);
            return in.readLine();
        }
    }

    public static void main(String[] args) {
        String host = "localhost";
        int port = 5000;

        if (args.length >= 1) host = args[0];
        if (args.length >= 2) {
            try {
                port = Integer.parseInt(args[1]);
            } catch (NumberFormatException ignored) {}
        }

        RPCClient client = new RPCClient(host, port);

        if (args.length >= 5) {
            String method = args[2];
            String n1 = args[3];
            String n2 = args[4];

            String req = method + " " + n1 + " " + n2;

            try {
                String resp = client.sendRequest(req);
                System.out.println("Server response: " + resp);
            } catch (IOException e) {
                System.err.println("Failed to send request: " + e.getMessage());
            }
            return;
        }

        System.out.println("RPC Client interactive mode (connects to "
                + host + ":" + port + ")");
        System.out.println("Enter: ADD 4 5   or type EXIT to quit.");

        Scanner sc = new Scanner(System.in);

        while (true) {
            System.out.print("> ");
            String line = sc.nextLine().trim();

            if (line.equalsIgnoreCase("EXIT") || line.equalsIgnoreCase("QUIT"))
                break;

            if (line.isEmpty()) continue;

            try {
                String resp = client.sendRequest(line);
                System.out.println("Server: " + resp);
            } catch (IOException e) {
                System.err.println("Error: " + e.getMessage());
            }
        }

        sc.close();
        System.out.println("Client exiting.");
    }
}
