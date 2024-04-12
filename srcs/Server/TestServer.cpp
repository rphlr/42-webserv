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

void	custom_close(int i)
{
	if (close(i) < 0)
		std::cerr << "close() failed with errno " << strerror(errno) << std::endl;
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
		custom_close(_listen_socket);
		std::cerr << "setsockopt" << std::endl;
		exit(-1);
	}
	if (fcntl(_listen_socket, F_SETFL, O_NONBLOCK) < 0) {
		custom_close(_listen_socket);
		std::cerr << "fcntl failed" << std::endl;
		exit(-1);
	}
	if (bind(_listen_socket, (sockaddr *)&_address, _addr_len) < 0) {
		std::cerr << "binding socket failed" << std::endl;
		custom_close(_listen_socket);
		exit(-1);
	}
	if (listen(_listen_socket, 255) < 0) {
		std::cerr << "listen() failed" << std::endl;
		custom_close(_listen_socket);
		exit(-1);
	}
	_new_socket = -1;
	FD_ZERO(&_master_fds);
	FD_ZERO(&_write_fds);
	FD_ZERO(&_read_fds);
	_max_nbr_of_sockets = _listen_socket;
	FD_SET(_listen_socket, &_master_fds);

	_timeout.tv_sec = 0;
	_timeout.tv_usec = 0;
}

void TestServer::run() {
	FD_COPY(&_master_fds, &_read_fds);

	int select_value = select(_max_nbr_of_sockets + 1, &_read_fds, &_write_fds, NULL, &_timeout);
	if (select_value < 0 || select_value > FD_SETSIZE) {
		std::cerr << "select() failed" << std::endl;
		for (int i = 0; i <= _max_nbr_of_sockets; i++) {
			if (FD_ISSET(i, &_master_fds) && i != _listen_socket) {				//double check if isset master or isset read
				FD_CLR(i, &_read_fds);
				FD_CLR(i, &_master_fds);
				custom_close(i);
			}
		}
		return;
	}
	for (int i = 0; i <= _max_nbr_of_sockets; i++) {
		if (FD_ISSET(i, &_read_fds) && i == _listen_socket) {
			std::cout << "New client on port: " << _port << std::endl;
			_new_socket = accept(_listen_socket, NULL, NULL);
			if (_new_socket < 0 && errno != EWOULDBLOCK) {
				std::cerr << "accept() failed for fd " << i << std::endl;
				return ;
			}
			else {
				FD_SET(_new_socket, &_master_fds);
				if (_max_nbr_of_sockets < _new_socket)
					_max_nbr_of_sockets = _new_socket;
			}
		}
		if (FD_ISSET(i, &_read_fds) && i != _listen_socket) {
			memset(_buffer, 0, sizeof(_buffer));
			int rc = recv(i, _buffer, 3000, 0);
			// std::cout << YELLOW << "Raw request:" << _buffer << RESET << "\n\n";
			if (rc <= 0) {
				std::cout << (rc == 0 ? "Client closed connection on fd " : "recv() error for fd ") << i << std::endl;
				FD_CLR(i, &_master_fds);
				FD_CLR(i, &_read_fds);
				custom_close(i);
				while (FD_ISSET(_max_nbr_of_sockets, &_master_fds) == false)
					_max_nbr_of_sockets--;
			}
			else {
				handler(i);
				FD_SET(i, &_write_fds);
			}
		}
		if (FD_ISSET(i, &_write_fds)) {
			std::cout << MAGENTA << "Send to handler: " << i << RESET << "\n\n";
			handler(i);
			FD_CLR(i, &_write_fds);
		}
	}
}

void TestServer::handler(int response_socket) {
	HandleRequest new_request(_buffer);
	new_request.handleRequest();
	std::string method = new_request.getMethod();

	if (method == "GET") {
		handleGet(new_request, response_socket);
	}
	else if (method == "POST") {
		// handlePost();
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

void TestServer::handleGet(HandleRequest &request, int response_socket) {
    std::string path = request.getPath();

	if (path.substr(path.size() - 4) == ".css") {
        handleCss(response_socket);
    } else if (path == "/home" || path == "/testHome.html") {
		// std::cout << "Handling home\n";
        handleRoot(response_socket);
    } else if (path == "/form" || path == "/testForm.html") {
		// std::cout << "Handling form\n";
        handleForm(response_socket);
	} else if (path == "/upload") {
		// std::cout << "Handling upload\n";
		handleUpload(response_socket);
    } else {
        handleError(response_socket);
    }
}

void TestServer::handleUpload(int response_socket) {
	std::string filePath = this->_rootPath + "/default_webpages/siteUpDownload.html";
	std::cout << "Root path: " << filePath << "\n\n";
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

	// std::cout << "Response: " << response << std::endl;
	send(_new_socket, response.c_str(), response.size(), 0);
}

std::string TestServer::determineCgiScriptPath(const std::string& requestPath) {
	std::cout << "Request path: " << requestPath << std::endl;
	return this->_rootPath  + "/cgi-bin" + requestPath.substr(8);
}

void TestServer::handlePost(HandleRequest &request, int response_socket) {
    std::string path = request.getPath();

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
	// std::cout << "Root path: " << filePath << "\n\n";
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

	if (send(response_socket, response.c_str(), response.size(), 0) < 0) {
		std::cout << "send() error on fd " << response_socket << std::endl;
		FD_CLR(response_socket, &_write_fds);
		custom_close(response_socket);
	}
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

	if (send(response_socket, response.c_str(), response.size(), 0) < 0) {
		std::cout << "send() error on fd " << response_socket << std::endl;
		// FD_CLR(response_socket, &_write_fds);
		custom_close(response_socket);
	}
}

void TestServer::handleForm(int response_socket)
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

	if (send(response_socket, response.c_str(), response.size(), 0) < 0) {
		std::cout << "send() error on fd " << response_socket << std::endl;
		// FD_CLR(response_socket, &_write_fds);
		custom_close(response_socket);
	}
}

void TestServer::handleError(int response_socket)
{
	std::string filePath = this->_rootPath + "/error_webpage/custom404.html";
	// std::cout << "Root path: " << filePath << std::endl;
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

	if (send(response_socket, response.c_str(), response.size(), 0) < 0){
		std::cout << "send() error on fd " << response_socket << std::endl;
		// FD_CLR(response_socket, &_write_fds);
		custom_close(response_socket);
	}
}

std::string &TestServer::getName()
{
	return _server_name;
}
