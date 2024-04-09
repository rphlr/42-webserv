import threading
import time

# Patch the standard library to use gevent
gevent.monkey.patch_all()

# Define the HTTP server IP and port
SERVER_IP = '127.0.0.1'
SERVER_PORT = 8888

# Number of clients to connect
NUM_CLIENTS = 5

# Duration (in seconds) for clients to stay connected
CONNECT_DURATION = 10

# Interval (in seconds) between message sends
MESSAGE_INTERVAL = 2

# Function to handle client connection and periodic message sending
def client_request(client_id):
    try:
        start_time = time.time()
        while time.time() - start_time < CONNECT_DURATION:
            try:
                # Send an HTTP POST request with a message
                message = "Hello from Client {}".format(client_id)
                url = "http://{}:{}/".format(SERVER_IP,SERVER_PORT)
                response = requests.post(url,data=message)

                # Print the response status and data
                print("Client {} - Response Status: {}".format(client_id, response.status_code))
                print("Client {} - Response Data: {}".format(client_id, response_text))

            except Exception as e:
                print("Client {} - Exception: {}".format(client_id, e))

            # Sleep before sending the next message
            gevent.sleep(MESSAGE_INTERVAL)

    except Exception as e:
        print("Error in client {}: {}".format(client_id, e))

# Create and start threads for each client
threads = []
for i in range(1, NUM_CLIENTS + 1):
    thread = threading.Thread(target=client_request, args=(i,))
    threads.append(thread)
    thread.start()

# Wait for all threads to complete
for thread in threads:
    thread.join()

print("All clients have finished.")

