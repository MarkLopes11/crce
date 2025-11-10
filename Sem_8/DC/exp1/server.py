import socket
import threading

# Server configuration
HOST = '127.0.0.1'  # localhost
PORT = 5555

# Create a TCP socket
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((HOST, PORT))
server.listen()

clients = []
nicknames = []

def broadcast(message, _client=None):
    """Send message to all connected clients."""
    for client in clients:
        if client != _client:
            client.send(message)

def handle_client(client):
    """Handle communication with a single client."""
    while True:
        try:
            message = client.recv(1024)
            if not message:
                break
            print(f"[MESSAGE] {message.decode('utf-8')}")
            broadcast(message, client)
        except:
            # Remove client on error
            index = clients.index(client)
            clients.remove(client)
            client.close()
            nickname = nicknames[index]
            broadcast(f"{nickname} left the chat.".encode('utf-8'))
            nicknames.remove(nickname)
            break

def receive_connections():
    """Accept new clients and start their threads."""
    print("[SERVER STARTED] Listening for connections...")
    while True:
        client, address = server.accept()
        print(f"[NEW CONNECTION] {str(address)} connected.")

        client.send("NICK".encode('utf-8'))
        nickname = client.recv(1024).decode('utf-8')
        nicknames.append(nickname)
        clients.append(client)

        print(f"Nickname of {address} is {nickname}")
        broadcast(f"{nickname} joined the chat!".encode('utf-8'))
        client.send("Connected to the server!".encode('utf-8'))

        # Start a new thread for this client
        thread = threading.Thread(target=handle_client, args=(client,))
        thread.start()

if __name__ == "__main__":
    receive_connections()
