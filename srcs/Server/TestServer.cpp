#include "../../includes/Server/TestServer.hpp"
#include "../../includes/Server/HandleRequest.hpp"
#include "../../includes/Server/HandleResponse.hpp"

// TestServer::TestServer() : SimpleServer( AF_INET, SOCK_STREAM, 0, 6545, INADDR_ANY, 10 ) {
// 	launch();
// }

TestServer::TestServer(Server &server) : SimpleServer( server ), _request("") {
	// Create a function to setup routes
	_routes["/home"] = &TestServer::handleRoot;
	_routes["/styles.css"] = &TestServer::handleCss;
	// _routes["/form"] = &TestServer::handleForm;

	launch();
}

void TestServer::launch() {
	while (true) {
		std::cout << "Waiting for a connection...\n";
		accepter();
		handler();
		responder();
		std::cout << "Done\n";
	}
}

void TestServer::accepter() {
	std::cout << "Accepting...\n";
	memset(_buffer, 0, 300 );
	struct sockaddr_in address = get_socket()->get_address();
	int addrlen = sizeof(address);
	_new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t*)&addrlen);
	read( _new_socket, _buffer, 300 );
}

void TestServer::handler() {
	std::cout << "Handling...\n";
	HandleRequest _request( _buffer);
	// _request.setRequest( _buffer );
	_request.handleRequest();
	std::string method = _request.getMethod();
	std::cout << "Method: " << method << std::endl;

	if (method == "GET") {
		std::cout << "GET request\n";
		handleGet(_request);
	}
	else if (method == "POST") {
		// handlePost();
		std::cout << "POST request\n";
	}
	else if (method == "DELETE") {
		// handleDelete();
		std::cout << "DELETE request\n";
	}
	else {
		std::cout << "Unsupported method\n";
	}
}

void TestServer::responder() {
	// HandleResponse response;
	// Check if the request is a GET request
	std::cout << "Responding...\n";
	std::string path = _request.getPath();
	if (_routes.find(path) != _routes.end()) {
		(this->*_routes[path])(_request);
	} else {
		handleError(_request);
	}
	shutdown(_new_socket, SHUT_RDWR);
}

void TestServer::handleGet(HandleRequest &request) {
	std::cout << "Handling GET request\n";
	std::string path = request.getPath();
	std::cout << "Path: " << path << std::endl;
	if (path == "/home") {
		handleRoot(request);
	} else if (path == "/styles.css") {
		handleCss(request);
	} else {
		handleError(request);
	}
}

void TestServer::handlePost(HandleRequest &request) {
	// Implement the POST request
	// send(_new_socket, response.c_str(), response.size(), 0);
	return;
}

void TestServer::handleDelete(HandleRequest &request) {
	// Implement the DELETE request
	// send(_new_socket, response.c_str(), response.size(), 0);
	return;
}

void TestServer::handleRoot(HandleRequest &request)
{
	std::ifstream file("/home/nate/Workspace/42projects/42-webserv/webpages/default_webpage/siteHome.html");
	if (!file.is_open())
	{
		// handle the error, e.g. by logging it and returning
		std::cerr << "Failed to open file handle-root\n";
		return;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string responseBody = buffer.str();

	std::string responseHeaders = "HTTP/1.1 200 Ok\r\n";
	responseHeaders += "Content-Type: text/html\r\n";
	responseHeaders += "Content-Length: " + std::to_string(responseBody.size()) + "\r\n";

	std::string response = responseHeaders + "\r\n" + responseBody;

	send(_new_socket, response.c_str(), response.size(), 0);
}

void TestServer::handleCss(HandleRequest &request)
{
	std::ifstream file("/home/nate/Workspace/42projects/42-webserv/webpages/default_webpage/styles.css");

	if (!file.is_open())
	{
		// handle the error, e.g. by logging it and returning
		std::cerr << "Failed to open handleCss\n";
		return;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string responseBody = buffer.str();

	std::string responseHeaders = "HTTP/1.1 200 Ok\r\n";
	responseHeaders += "Content-Type: text/css\r\n";
	responseHeaders += "Content-Length: " + std::to_string(responseBody.size()) + "\r\n";

	std::string response = responseHeaders + "\r\n" + responseBody;

	send(_new_socket, response.c_str(), response.size(), 0);
}

void TestServer::handleError(HandleRequest &request)

{
	std::ifstream file("/home/nate/Workspace/42projects/42-webserv/webpages/error_webpages/custom_404.html");

	if (!file.is_open())
	{
		std::cerr << "Failed to open delete\n";
		return;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string responseBody = buffer.str();

	std::string responseHeaders = "HTTP/1.1 404 Not Found\r\n";
	responseHeaders += "Content-Type: text/html\r\n";
	responseHeaders += "Content-Length: " + std::to_string(responseBody.size()) + "\r\n";

	std::string response = responseHeaders + "\r\n" + responseBody;

	send(_new_socket, response.c_str(), response.size(), 0);
}


/* TESTING */

// void TestServer::launch() 
// {
// 	fd_set current_sockets, ready_sockets;
// 	FD_ZERO(&current_sockets); // Clears the fd_set

// 	// Add listening socket to the set
// 	FD_SET(get_socket()->get_sock(), &current_sockets);

// 	int max_fd = get_socket()->get_sock();

// 	while (true) 
// 	{
// 		ready_sockets = current_sockets;

// 		if (select(max_fd + 1, &ready_sockets, NULL, NULL, NULL) == -1) 
// 		{
// 			std::cerr << "Select error: " << strerror(errno);
// 			break;
// 		}

// 		// Check each socket for readiness
// 		for(int i = 0; i <= max_fd; i++) 
// 		{
// 			if (FD_ISSET(i, &ready_sockets)) 
// 			{
// 				if (i == max_fd) 
// 				{
// 					// Accept new connection
// 					accepter();
// 				} 
// 				else 
// 				{
// 					handler();

// 					if(strlen(_buffer) !=  0) // Check if there's data in the buffer
// 					{
// 						responder();
// 					}
// 				}
// 			}
// 		}
// 	}
// }
