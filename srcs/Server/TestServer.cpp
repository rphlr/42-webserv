#include "../../includes/Server/TestServer.hpp"

TestServer::TestServer( Server &server ) {
	u_long interface;
	_port = server.getPort();
	_address.sin_family = AF_INET;
	_address.sin_port = htons(_port);
	interface = inet_pton(AF_INET, server.getHost().c_str(), &_address.sin_addr);
	if (interface <= 0)
		std::cout << "loopback host or not valid" << std::endl;
	_addr_len = sizeof(_address);

	// Create a function to setup routes
	_routes["/home"] = &TestServer::handleRoot;
	_routes["/styles.css"] = &TestServer::handleCss;
	// _routes["/form"] = &TestServer::handleForm;

	init();
}

void TestServer::init()
{
	int on = 1;

	_listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_listen_socket < 0) {
		std::cerr << "initializing server socket failed" << std::endl;
		exit(-1);
	}
	if (setsockopt(_listen_socket, SOL_SOCKET,  SO_REUSEADDR,
		(char *)&on, sizeof(on)) < 0) {
		close(_listen_socket);
		std::cerr << "setsockopt" << std::endl;
		exit(-1);
	}
	if (fcntl(_listen_socket, F_SETFL, O_NONBLOCK) < 0) {
		close(_listen_socket);
		std::cerr << "fcntl failed" << std::endl;
		exit(-1);
	}
	if (bind(_listen_socket, (sockaddr *)&_address, _addr_len) < 0) {
		std::cerr << "binding socket failed" << std::endl;
		close(_listen_socket);
		exit(-1);
	}
	if (listen(_listen_socket, 1) < 0) {
		std::cerr << "listen() failed" << std::endl;
		close(_listen_socket);
		exit(-1);
	}
	_new_socket = -1;
	FD_ZERO(&_master_read_fds);
	_max_sockets = _listen_socket;
	FD_SET(_listen_socket, &_master_read_fds);

	_timeout.tv_sec = 1;
	_timeout.tv_usec = 0;
}

void TestServer::run() {
	std::cout << "Waiting for a connection on port: " << _port << std::endl;
	FD_ZERO(&_write_fds);
	FD_ZERO(&_working_read_fds);
	memcpy(&_working_read_fds, &_master_read_fds, sizeof(_master_read_fds));

	int select_value = select(_max_sockets + 1, &_working_read_fds, &_write_fds, NULL, &_timeout);
	if (select_value < 0) {
		std::cerr << "select() failed" << std::endl;
		for (int i = 0; i <= _max_sockets; i++) {
			if (FD_ISSET(i, &_master_read_fds) && i != _listen_socket)
			close(i);
		}
		return ;
	}
	else if (select_value == 0) {
		std::cerr << "select() timeout" << std::endl;
		return ;
	}
	for (int i = 0; i <= _max_sockets; i++) {
		if (FD_ISSET(i, &_working_read_fds) && i == _listen_socket) {
			_new_socket = accept(_listen_socket, NULL, NULL);
			if (_new_socket < 0 && errno != EWOULDBLOCK) {
				std::cerr << "accept() failed" << std::endl;
				return ;
			}
			if (fcntl(_new_socket, F_SETFL, O_NONBLOCK) < 0) {
				std::cerr << "fcntl failed" << std::endl;
				close(_new_socket);
			}
			FD_SET(_new_socket, &_master_read_fds);
			if (_max_sockets < _new_socket)
				_max_sockets = _new_socket;
		}
		if (FD_ISSET(i, &_working_read_fds) && i != _listen_socket) {
			memset(_buffer, 0, sizeof(_buffer));
			int rc = recv(i, _buffer, 3000, 0);
			std::cout << _buffer << std::endl;
			if (rc <= 0) {
				std::cerr << "recv() failed or client closed connection" << std::endl;
				close(i);
				FD_CLR(i, &_master_read_fds);
				FD_CLR(i, &_write_fds);
			}
			else {
				FD_SET(i, &_write_fds);
				std::cout << "send to handler: " << i << std::endl;
				handler(i);
				// if (send(i, "hello", 5, 0) < 0)
				// 	close(i);
			}
		}
	}
}

void TestServer::handler(int response_socket) {
	std::cout << "Handling...\n";
	HandleRequest new_request(_buffer);
	new_request.handleRequest();
	std::string method = new_request.getMethod();
	std::cout << "Method: " << method << std::endl;

	if (method == "GET") {
		std::cout << "GET request\n";
		handleGet(new_request, response_socket);
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

void TestServer::handleGet(HandleRequest &new_request, int response_socket) {
	std::cout << "Handling GET request\n";
	std::string path = new_request.getPath();
	std::cout << "Path: " << path << std::endl;
	if (path == "/home") {
		std::cout << "GET: handle root\n";
		handleRoot(response_socket);
	} else if (path == "/styles.css") {
		handleCss(response_socket);
	} else {
		handleError(response_socket);
	}
}

void TestServer::handlePost(HandleRequest &new_request, int response_socket) {
	(void)new_request;
	(void)response_socket;
	// Implement the POST request
	// send(_new_socket, response.c_str(), response.size(), 0);
	return;
}

void TestServer::handleDelete(HandleRequest &new_request, int response_socket) {
	(void)new_request;
	(void)response_socket;
	// Implement the DELETE request
	// send(_new_socket, response.c_str(), response.size(), 0);
	return;
}

void TestServer::handleRoot(int response_socket)
{
	std::ifstream file("/Users/ckarl/Desktop/webserv/webpages/default_webpage/siteHome.html");
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

	if (send(response_socket, response.c_str(), response.size(), 0) < 0)
		close(response_socket);
}

void TestServer::handleCss(int response_socket)
{
	std::ifstream file("/Users/ckarl/Desktop/webserv/webpages/default_webpage/styles.css");

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

	if (send(response_socket, response.c_str(), response.size(), 0) < 0)
		close(response_socket);
}

void TestServer::handleError(int response_socket)

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

	if (send(response_socket, response.c_str(), response.size(), 0) < 0)
		close(response_socket);
}

// void TestServer::responder() {
// 	HandleResponse response;
// 	Check if the request is a GET request
// 	std::cout << "Responding...\n";
// 	std::string path = _request.getPath();
// 	if (_routes.find(path) != _routes.end()) {
// 		(this->*_routes[path])(_request);
// 	} else {
// 		handleError(_request);
// 	}
// 	shutdown(_new_socket, SHUT_RDWR);
// }


/* TESTING */

// void TestServer::run()
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
