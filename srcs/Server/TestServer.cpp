#include "../../includes/Server/TestServer.hpp"

TestServer::TestServer( Server &server ) : _request("") {
	u_long interface;
	_end_server = false;
	_close_connection = false;
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
	launch();
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
	//bind the socket
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
	FD_ZERO(&_working_read_fds);
	_max_sockets = _listen_socket;
	FD_SET(_listen_socket, &_master_read_fds);

	_timeout.tv_sec = 180;
	_timeout.tv_usec = 0;
}

void TestServer::launch() {

	while (_end_server == false) {
		std::cout << "Waiting for a connection...\n";
		memcpy(&_working_read_fds, &_master_read_fds, sizeof(_master_read_fds));

		if (select(_max_sockets + 1, &_working_read_fds, NULL, NULL, &_timeout) <= 0) {
			std::cerr << "select() failed or timeout" << std::endl;
			break;
		}
		for (int i = 0; i <= _max_sockets; i++) {
			if (FD_ISSET(i, &_working_read_fds) && i == _listen_socket) {
				_new_socket = accept(_listen_socket, NULL, NULL);
				if (_new_socket < 0 && errno != EWOULDBLOCK) {
					std::cerr << "accept() failed" << std::endl;
					_end_server = true;
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
				}
				else {
					std::cout << "send to handler" << std::endl;
					// handler();
					if (send(i, "hello", 5, 0) < 0)
						close(i);
					// while (FD_ISSET(_max_sockets, &_master_read_fds) == false)
					// 	_max_sockets--;
				}
			}
			// std::cout << "maxsockets: " << _max_sockets << std::endl;
		}
	}
	std::cout << "Done\n";
	// responder();
	for (int i = 0; i <= _max_sockets; i++) {
		if (FD_ISSET(i, &_master_read_fds) && i != _listen_socket)
			close(i);
	}
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
		std::cout << "GET: handle root\n";
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
