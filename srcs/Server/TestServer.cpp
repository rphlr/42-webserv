#include "../../includes/Server/TestServer.hpp"
#include "../../includes/Server/HandleRequest.hpp"
#include "../../includes/Server/HandleResponse.hpp"
#include "../../includes/Server/HandleCGI.hpp"

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

TestServer::TestServer( Server &server ) {

	char *cwd = getcwd(NULL, 0);
	this->_rootPath = std::string(cwd) + "/" + server.getRoot();
	free(cwd);
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
	_routes["/upload"] = &TestServer::handleUpload;
	_routes["/form"] = &TestServer::handleForm;

	init();
}

// TestServer::TestServer(Server &server) : SimpleServer( server ), _request("") {
// 	// Create a function to setup routes
// 	_routes["/home"] = &TestServer::handleHome;
// 	_routes["/form"] = &TestServer::handleForm;
// 	_routes["/styles.css"] = &TestServer::handleCss;
// 	_routes["/upload"] = &TestServer::handlePost;
// 	// _routes["/form"] = &TestServer::handleForm;

// 	launch();
// }

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
	if (listen(_listen_socket, 255) < 0) {
		std::cerr << "listen() failed" << std::endl;
		close(_listen_socket);
		exit(-1);
	}
	_new_socket = -1;
	FD_ZERO(&_master_read_fds);
	_max_sockets = _listen_socket;
	FD_SET(_listen_socket, &_master_read_fds);

	_timeout.tv_sec = 0;
	_timeout.tv_usec = 0;
}

// void TestServer::launch() {
// 	while (true) {
// 		std::cout << "Waiting for a connection...\n\n";
// 		accepter();
// 		handler();
// 		responder();
// 	}
// }

void TestServer::run() {
	std::cout << "Waiting for a connection on port: " << _port << std::endl;
	// FD_ZERO(&_write_fds);
	// FD_ZERO(&_working_read_fds);
	// memcpy(&_working_read_fds, &_master_read_fds, sizeof(_master_read_fds));
	FD_COPY(&_master_read_fds, &_working_read_fds);

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
		// std::cerr << "select() timeout" << std::endl;
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
		handlePost(new_request, response_socket);
	}
	else if (method == "DELETE") {
		// handleDelete();
		std::cout << "DELETE request\n";
	}
	else {
		std::cout << "Unsupported method\n";
	}
}

// void TestServer::accepter() {
// 	memset(_buffer, 0, 300 );
// 	struct sockaddr_in address = get_socket()->get_address();
// 	int addrlen = sizeof(address);
// 	_new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t*)&addrlen);
// 	read( _new_socket, _buffer, 300 );
// }

// void TestServer::handler() {
// 	std::cout << "Handling...\n";
// 	HandleRequest _request( _buffer);
// 	// _request.setRequest( _buffer );
// 	_request.handleRequest();
// 	std::string method = _request.getMethod();

// 	std::cout << MAGENTA;
// 	if (method == "GET") {
// 		std::cout << "GET request\n";
// 		handleGet(_request);
// 	}
// 	else if (method == "POST") {
// 		// handlePost();
// 		std::cout << "POST request\n";
// 		handlePost(_request);
// 	}
// 	else if (method == "DELETE") {
// 		// handleDelete();
// 		std::cout << "DELETE request\n";
// 	}
// 	else {
// 		std::cout << "Unsupported method\n";
// 	}
// 	std::cout << "\n" << RESET;
// }

// void TestServer::handlePost(HandleRequest &request) {
//     // Implémentez ici le traitement spécifique aux données POST.
//     std::string contentType = request.getHeader("Content-Type");
//     // Vérifiez si c'est une requête de téléchargement de fichier
//     if (contentType.find("multipart/form-data") != std::string::npos) {
//         // Vous devez parser le corps de la requête pour extraire le fichier
//         // et le traiter selon les spécifications de votre application.
        
//         // Après avoir traité le fichier, envoyez une réponse appropriée.
//         std::string responseBody = "<html><body><h1>Fichier téléchargé avec succès</h1></body></html>";
//         std::string responseHeaders = "HTTP/1.1 200 OK\r\n";
//         responseHeaders += "Content-Type: text/html\r\n";
//         responseHeaders += "Content-Length: " + std::to_string(responseBody.size()) + "\r\n\r\n";

//         std::string response = responseHeaders + responseBody;
//         send(_new_socket, response.c_str(), response.size(), 0);
//     } else {
//         // Si ce n'est pas une requête de téléchargement de fichier, traitez-la normalement.
//     }
// }

// void TestServer::responder() {
// 	// HandleResponse response;
// 	// Check if the request is a GET request
// 	std::cout << YELLOW;
// 	std::cout << "Responding...\n";
// 	std::string path = _request.getPath();
// 	if (_routes.find(path) != _routes.end()) {
// 		(this->*_routes[path])(_request);
// 	} else {
// 		handleError(_request);
// 	}
// 	shutdown(_new_socket, SHUT_RDWR);
// 	std::cout << "\n" << RESET;
// }

void TestServer::handleGet(HandleRequest &request, int response_socket) {
    std::cout << "Handling GET request\n";
    std::string path = request.getPath();
    std::cout << "Path: " << path << std::endl;

	if (path.substr(path.size() - 4) == ".css") {
		std::cout << "Handling css\n";
        handleCss(response_socket);
    } else if (path == "/home" || path == "/testHome.html") {
		std::cout << "Handling home\n";
        handleRoot(response_socket);
    } else if (path == "/form" || path == "/testForm.html") {
		std::cout << "Handling form\n";
        handleForm(response_socket);
	} else if (path == "/upload") {
		std::cout << "Handling upload\n";
		handleUpload(response_socket);
    } else {
        handleError(response_socket);
    }
}

void TestServer::handleUpload(int response_socket) {
	(void) response_socket;
	std::string filePath = this->_rootPath + "/default_webpages/siteUpDownload.html";
	std::cout << "Root path: " << filePath << std::endl;
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "Failed to open handleUpload\n";
		return;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string responseBody = buffer.str();

	std::string responseHeaders = "HTTP/1.1 200 Ok\r\n";
	responseHeaders += "Content-Type: text/html\r\n";
	responseHeaders += "Content-Length: " + std::to_string(responseBody.size()) + "\r\n";

	std::string response = responseHeaders + "\r\n" + responseBody;

	std::cout << "Response: " << response << std::endl;
	send(_new_socket, response.c_str(), response.size(), 0);
}

std::string TestServer::determineCgiScriptPath(const std::string& requestPath) {
	std::cout << "Request path: " << requestPath << std::endl;
	return this->_rootPath  + "/cgi-bin" + requestPath.substr(8);
}

void TestServer::handlePost(HandleRequest &request, int response_socket) {
	(void) response_socket;
    std::cout << "Handling POST request\n";
    std::string path = request.getPath();
    std::cout << "Path: " << path << std::endl;

    if (path.substr(0, 9) == "/cgi-bin/") {
		std::cout << "Handling CGI\n";
        std::map<std::string, std::string> cgiEnv;
        cgiEnv["REQUEST_METHOD"] = "POST";

        std::string scriptPath = determineCgiScriptPath(path);

		std::cout << "Script path: " << scriptPath << std::endl;
        std::string postData = request.getBody();
		std::cout << "Post data: " << postData << std::endl;
        cgiEnv["CONTENT_LENGTH"] = std::to_string(postData.size());
		std::cout << "Content length: " << cgiEnv["CONTENT_LENGTH"] << std::endl;
        cgiEnv["CONTENT_TYPE"] = request.getHeader("Content-Type");

		std::cout << "Script path: " << scriptPath << std::endl;
        HandleCGI cgiHandler(scriptPath, cgiEnv, postData);
		std::cout << "Executing CGI\n";
        std::string cgiOutput = cgiHandler.execute();

		std::cout << "CGI output: " << cgiOutput << std::endl;
        send(_new_socket, cgiOutput.c_str(), cgiOutput.size(), 0);
    }
}


// void TestServer::handlePost(HandleRequest &request) {
// 	// implement the POST request
// 	std::string contentType = request.getHeader("Content-Type");
// 	std::string path = request.getPath();
// 	std::string responseBody;
// 	std::string responseHeaders;
// 	std::string response;


// }

void TestServer::handleDelete(HandleRequest &request, int response_socket) {
	(void) request;
	(void) response_socket;
	// Implement the DELETE request
	// send(_new_socket, response.c_str(), response.size(), 0);
	return;
}

void TestServer::handleRoot(int response_socket)
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

	if (send(response_socket, response.c_str(), response.size(), 0) < 0)
		close(response_socket);
}

void TestServer::handleCss(int response_socket)
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

	if (send(response_socket, response.c_str(), response.size(), 0) < 0)
		close(response_socket);
}

void TestServer::handleForm(int response_socket)
{
	(void) response_socket;
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

void TestServer::handleError(int response_socket)
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

	if (send(response_socket, response.c_str(), response.size(), 0) < 0)
		close(response_socket);
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