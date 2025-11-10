import socket
import threading

# Server connection details
HOST = '127.0.0.1'
PORT = 5555

nickname = input("Choose your nickname: ")

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((HOST, PORT))

def receive_messages():
    """Receive messages from the server."""
    while True:
        try:
            message = client.recv(1024).decode('utf-8')
            if message == 'NICK':
                client.send(nickname.encode('utf-8'))
            else:
                print(message)
        except:
            print("An error occurred! Disconnecting...")
            client.close()
            break

def send_messages():
    """Send messages to the server."""
    while True:
        message = f"{nickname}: {input('')}"
        client.send(message.encode('utf-8'))

# Start threads for send and receive
receive_thread = threading.Thread(target=receive_messages)
receive_thread.start()

write_thread = threading.Thread(target=send_messages)
write_thread.start()

