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

/* *********************************** *
**  Core functionnalities ************ *
* *********************************** */
ListeningSocket * TestServer::get_socket() {
	return _socket;
}

// /* *********************************** *
// **  Canonical Form ******************* *
// * *********************************** */
TestServer::TestServer( Server &server ) : _request("") {
	int on = 1;
	_new_socket = -1;
	_port = server.getPort();
	u_long interface;
	if (!server.getHost().compare("127.0.0.1")){
		interface = INADDR_LOOPBACK;
	}
	else {
		interface = inet_addr(server.getHost().c_str());
	}
	int bklg = 10;
	// _socket = new ListeningSocket( domain, service, protocol, port, interface, bklg );
		// Create a function to setup routes
	_listen_socket = socket(AF_INET, SOCK_STREAM, 0);

	_routes["/home"] = &TestServer::handleRoot;
	_routes["/styles.css"] = &TestServer::handleCss;
	_routes["/upload"] = &TestServer::handlePost;
	// _routes["/form"] = &TestServer::handleForm;

	// memset(&_address, 0, sizeof(_address));
	_address.sin_family = AF_INET;
	_address.sin_port = htons(_port);
	_address.sin_addr.s_addr = htonl(interface);

	_rc = setsockopt(_listen_socket, SOL_SOCKET,  SO_REUSEADDR,
		(char *)&on, sizeof(on));
	if (_rc < 0) {
		close(_listen_socket);
		exit(-1);
	}
	_rc = bind(_listen_socket,
		(struct sockaddr *)&_address, sizeof(_address));
	if (_rc < 0) {
		close(_listen_socket);
		exit(-1);
	}
	memset(_fds, 0 , sizeof(_fds));
	_fds[0].fd = _listen_socket;
	_fds[0].events = POLLIN;
	_timeout = 180000;

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
		handlePost(_request);
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

	if (path.substr(path.size() - 4) == ".css") {
		std::cout << "Handling css\n";
        handleCss(request);
    } else if (path == "/home" || path == "/testHome.html") {
		std::cout << "Handling home\n";
        handleHome(request);
    } else if (path == "/form" || path == "/testForm.html") {
		std::cout << "Handling form\n";
        handleForm(request);
	} else if (path == "/upload") {
		std::cout << "Handling upload\n";
		handleUpload(request);
    } else {
        handleError(request);
    }
}

void TestServer::handleUpload(HandleRequest &request) {
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

void TestServer::handlePost(HandleRequest &request) {
    std::cout << "Handling POST request\n";
    std::string path = request.getPath();
    std::cout << "Path: " << path << std::endl;

    if (path.substr(0, 9) == "/cgi-bin/" ) {
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
