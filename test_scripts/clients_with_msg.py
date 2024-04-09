import http.client
import threading
import time

# Define the HTTP server IP and port
SERVER_IP = '127.0.0.1'
SERVER_PORT = 8888

# Number of clients to connect
NUM_CLIENTS = 10

# Duration (in seconds) for clients to stay connected
CONNECT_DURATION = 5

# Interval (in seconds) between message sends
MESSAGE_INTERVAL = 2

# Function to handle client connection and periodic message sending
def client_request(client_id):
    try:
        # Create an HTTP connection to the server
        conn = http.client.HTTPConnection(SERVER_IP, SERVER_PORT)

        print("Client {} connected to server {}:{}".format(client_id, SERVER_IP, SERVER_PORT))

        # Send messages periodically while the client is connected
        start_time = time.time()
        while time.time() - start_time < CONNECT_DURATION:
            try:
                # Send an HTTP POST request with a message
                message = "Hello from Client {}".format(client_id)
                conn.request("POST", "/", body=message)

                # Get the response from the server
                response = conn.getresponse()

                # Print the response status and data
                print("Client {} - Response Status: {}".format(client_id, response.status))
                print("Client {} - Response Data: {}".format(client_id, response.read().decode()))

                # Sleep before sending the next message
                time.sleep(MESSAGE_INTERVAL)

            except http.client.HTTPException as http_err:
                print("Client {} - HTTP Exception: {}".format(client_id, http_err))
                break  # Break the loop on HTTP exception

            except Exception as e:
                print("Client {} - Exception: {}".format(client_id, e))
                break  # Break the loop on any other exception

        # Close the connection after the specified duration
        conn.close()
        print("Client {} disconnected from server".format(client_id))

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

