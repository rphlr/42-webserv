import socket
import threading
import time
from signal import signal, SIGPIPE, SIG_DFL
#signal(SIGPIPE,SIG_DFL)

SERVER_HOST = '127.0.0.1'
SERVER_PORTS = [8888, 8000, 8080, 8008, 8800]
NUM_CLIENTS = 10

def client_task(server_port):
    try:
        # Create a new socket for the client
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        
        # Connect to the server
        client_socket.connect((SERVER_HOST, server_port))
        print("Client connected to {}:{}".format(SERVER_HOST, server_port))

        # Optionally send data to the server
        client_socket.sendall(b"Hello from client")

        # Optionally receive data from the server
        response = client_socket.recv(1024)
        print("Received from server: {}".format(response.decode()))

        # Close the client socket
        time.sleep(6)
        client_socket.close()
        print("Client socket closed")

    except Exception as e:
        print("Exception in client task: {}".format(e))

if __name__ == "__main__":
    # Create and start multiple client threads
    threads = []
    for port in SERVER_PORTS:
        for i in range(NUM_CLIENTS):
            thread = threading.Thread(target=client_task, args=(port,))
            thread.start()
            threads.append(thread)

    # Wait for all client threads to complete
    for thread in threads:
        thread.join()

    print("All clients have completed")

