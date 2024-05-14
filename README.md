# Webserv 🌐

Welcome to our Webserv project for 42! This README provides an overview of our HTTP server implementation, configuration details, and how to get started using our server.

## What is an HTTP Server?

An HTTP server is software designed to accept requests from clients and send them responses over the internet or a network. Typically, it serves web pages to users in response to requests which include methods like GET, POST, DELETE, etc.

## Project Overview

Our project, Webserv, is designed to simulate the behavior of real HTTP servers but on a smaller scale. It can handle multiple server configurations simultaneously up to a limit of 1024 servers due to our choice of using the `select()` function. The key features of our implementation are:

- **Multiple Servers:** Up to 1024 servers can be managed concurrently.
- **Chunked Data Transmission:** Data is sent in chunks, making it suitable for transmitting large amounts of data.
- **Strict Configuration:** The server checks for duplicate ports or server names and will not start if these are found.
- **Error Handling:** We have implemented a basic set of error pages (403, 404, 405, 413, and 501) with easy extensions for more.
- **CGI Support:** Handles execution of Python, Perl, PHP, or Bash scripts through CGI on `/cgi`.
- **File Uploads:** Files can be uploaded to a specific directory and handled through CGI scripts.
- **Account Management:** Basic user account actions can be handled through `/register` and `/login`.

## Project Achievements

We are proud to announce that our Webserv project received the highest possible mark, scoring a 125% at evaluation. This reflects our commitment to quality, functionality, and adherence to project specifications.

## Robustness Testing

Our server's robustness was tested using the Siege stress tester. After several extended test sessions, Webserv showed excellent reliability, sustaining a 100% success rate across the tests. This demonstrates our server's ability to handle high loads and maintain performance under stress.

## Configuration

Our server configurations are detailed in a separate file. Below is an example configuration for our server:

```yaml
server:
        server_name: example
        port: 1234
        host: 127.0.0.1
        max_body_size: 23854644
        root: example_website_path
        default_file: index.html
        error_page:
                403: /error_webpages/403.html
                404: /error_webpages/404.html
                405: /error_webpages/405.html
                413: /error_webpages/413.html
                501: /error_webpages/501.html
        location:
                path: /
                methods: GET, POST
                redirect: /index.html
# Multiple server blocks can be configured similarly
```

## How to Use

To start using Webserv, follow these steps:

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/rphlr/42-webserv [nameOfFolder]
   ```
2. **Compile the Server:**
   ```bash
   make
   ```
3. **Run the Server:**
   ```bash
   ./webserv [configuration-file-path]
   ```
   ```bash
   make run
   ```

## Testing

For testing purposes with `curl`, you can use the following commands:

- **Upload a File:**
  ```bash
  curl -X POST "file=@/Path/to/File" http://localhost:port/cgi?upload=true
  ```
- **Execute a CGI Script:**
  ```bash
  curl -X GET http://localhost:port/cgi?file=/path/to/file
  ```
- **Delete a File:**
  ```bash
  curl -X DELETE http://localhost:port/default_webpages?file=FileToDelete
  ```

## Contribution

Feel free to contribute to the project by submitting pull requests or by reporting bugs.

Thank you for checking out our Webserv project! We hope it serves as a solid base for learning more about the inner workings of HTTP servers. 🚀
