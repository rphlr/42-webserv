#include "../../includes/Server/TestServer.hpp"
#include "../../includes/Server/HandleRequest.hpp"
#include "../../includes/Server/HandleResponse.hpp"

#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BRIGHT_BLACK "\033[90m"
#define BRIGHT_RED "\033[91m"

#include <cstdlib> 

// TestServer::TestServer() : SimpleServer( AF_INET, SOCK_STREAM, 0, 6545, INADDR_ANY, 10 ) {
// 	launch();
// }

TestServer::TestServer(Server &server) : SimpleServer( server ), _request("") {
	// Create a function to setup routes
	_routes["/home"] = &TestServer::handleHome;
	_routes["/form"] = &TestServer::handleForm;
	_routes["/styles.css"] = &TestServer::handleCss;
	// _routes["/form"] = &TestServer::handleForm;

	launch();
}

void TestServer::launch() {
	while (true) {
		std::cout << "Waiting for a connection...\n\n";
		accepter();
		handler();
		responder();
	}
}

void TestServer::accepter() {
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

	std::cout << MAGENTA;
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
	std::cout << "\n" << RESET;
}

void TestServer::responder() {
	// HandleResponse response;
	// Check if the request is a GET request
	std::cout << YELLOW;
	std::cout << "Responding...\n";
	std::string path = _request.getPath();
	if (_routes.find(path) != _routes.end()) {
		(this->*_routes[path])(_request);
	} else {
		handleError(_request);
	}
	shutdown(_new_socket, SHUT_RDWR);
	std::cout << "\n" << RESET;
}

void TestServer::handleGet(HandleRequest &request) {
	std::cout << "Handling GET request\n";
	std::string path = request.getPath();
	std::cout << "Path: " << path << std::endl;
	if (path == "/home" || path == "/testHome.html") {
		handleHome(request);
	} else if (path == "/styles.css") {
		handleCss(request);
	} else if (path == "/form" || path == "/testForm.html") {
		handleForm(request);
	}
	else {
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

void TestServer::handleHome(HandleRequest &request)
{
	std::string filePath = this->_rootPath + "/default_webpages/siteHome.html";
	std::cout << "Root path: " << filePath << std::endl;
	std::ifstream file(filePath);
	// std::ifstream file("/home/nate/Workspace/42projects/42-webserv/webpages/default_webpage/siteHome.html");
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
	std::string filePath = this->_rootPath + "/default_webpages/styles.css";
	std::ifstream file(filePath);
	// std::ifstream file("/home/nate/Workspace/42projects/42-webserv/webpages/default_webpage/styles.css");

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

void TestServer::handleForm(HandleRequest &request)
{
	std::string filePath = this->_rootPath + "/default_webpages/siteForm.html";
	std::ifstream file(filePath);
	// std::ifstream file("/home/nate/Workspace/42projects/42-webserv/webpages/default_webpage/siteForm.html");
	if (!file.is_open())
	{
		// handle the error, e.g. by logging it and returning
		std::cerr << "Failed to open handleForm\n";
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

void TestServer::handleError(HandleRequest &request)
{
    // std::string responseBody = "404 Not Found\nThe requested URL " + request.getPath() + " was not found on this server.";

    // std::string responseHeaders = "HTTP/1.1 404 Not Found\r\n";
    // responseHeaders += "Content-Type: text/plain\r\n";
    // responseHeaders += "Content-Length: " + std::to_string(responseBody.size()) + "\r\n";

    // std::string response = responseHeaders + "\r\n" + responseBody;

    // send(_new_socket, response.c_str(), response.size(), 0);

	std::string filePath = this->_rootPath + "/error_webpages/custom404.html";\
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "Failed to open handleError\n";
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
