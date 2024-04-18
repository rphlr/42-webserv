import requests
from multiprocessing import Process, freeze_support

url = "http://localhost:8888/home"

# Function to make HTTP GET request to the URL
def open_webpage(client_id):
    retries = 3
    for _ in range(retries):
        try:
            response = requests.get(url)
            print(f"Client {client_id} Request to {url} - Status Code: {response.status_code}")
            break
        except requests.exceptions.RequestException as e:
            print(f"Client {client_id} Error making request to {url}: {e}")
            time.sleep(2)
    #print(f"Client {client_id} - Failed to make request to {url} after {retries} retries")

if __name__ == '__main__':
    # Number of times to open the webpage simultaneously
    num_clients = 10

    # List to hold processes
    processes = []

    # Create processes to send simultaneous requests
    for i in range(num_clients):
        process = Process(target=open_webpage, args=(i+1,))
        processes.append(process)
        process.start()

    # Wait for all processes to complete
    for process in processes:
        process.join()

    print(f"All {num_clients} requests to {url} completed.")

