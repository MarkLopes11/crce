import java.io.*;
import java.net.*;

public class RPCServer {

    private final int port;
    private final CalculatorImpl calc = new CalculatorImpl();

    public RPCServer(int port) {
        this.port = port;
    }

    public void start() throws IOException {
        ServerSocket serverSocket = new ServerSocket(port);
        System.out.println("RPC Server started on port " + port);

        while (true) {
            Socket clientSocket = serverSocket.accept();
            new Thread(new ClientHandler(clientSocket, calc)).start();
        }
    }

    public static void main(String[] args) {
        int port = 5000;

        if (args.length >= 1) {
            try {
                port = Integer.parseInt(args[0]);
            } catch (NumberFormatException ignored) {}
        }

        try {
            new RPCServer(port).start();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static class ClientHandler implements Runnable {
        private final Socket socket;
        private final CalculatorImpl calc;

        public ClientHandler(Socket socket, CalculatorImpl calc) {
            this.socket = socket;
            this.calc = calc;
        }

        @Override
        public void run() {
            try (
                BufferedReader in = new BufferedReader(
                        new InputStreamReader(socket.getInputStream())
                );
                PrintWriter out = new PrintWriter(socket.getOutputStream(), true)
            ) {
                String request = in.readLine();
                if (request == null) return;

                String resp = handleRequest(request.trim());
                out.println(resp);

            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                try {
                    socket.close();
                } catch (IOException ignored) {}
            }
        }

        private String handleRequest(String req) {
            String[] parts = req.split("\\s+");

            if (parts.length < 3) {
                return "ERR Invalid request. Use: ADD|SUB|MUL|DIV <num1> <num2>";
            }

            String method = parts[0].toUpperCase();
            double a, b;

            try {
                a = Double.parseDouble(parts[1]);
                b = Double.parseDouble(parts[2]);
            } catch (NumberFormatException e) {
                return "ERR Number format error";
            }

            try {
                double result;

                switch (method) {
                    case "ADD": result = calc.add(a, b); break;
                    case "SUB": result = calc.sub(a, b); break;
                    case "MUL": result = calc.mul(a, b); break;
                    case "DIV": result = calc.div(a, b); break;
                    default: return "ERR Unknown method: " + method;
                }

                return "OK " + result;

            } catch (ArithmeticException ae) {
                return "ERR " + ae.getMessage();
            } catch (Exception e) {
                return "ERR Internal server error";
            }
        }
    }
}
