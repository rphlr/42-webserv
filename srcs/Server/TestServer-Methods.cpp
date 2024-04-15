#include "../../includes/Server/TestServer.hpp"
#include "../../includes/Server/HandleRequest.hpp"
#include "../../includes/Server/HandleResponse.hpp"
#include "../../includes/Server/HandleCGI.hpp"

void TestServer::handleNotImplemented(HandleRequest &new_request, int response_socket)
{
	std::string filePath = this->_rootPath + "/error_webpage/custom501.html";
	// std::cout << "Root path: " << filePath << std::endl;
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "Failed to open handleNotImplemented\n";
		return;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string responseBody = buffer.str();

	std::string responseHeaders = "HTTP/1.1 501 Not Implemented\r\n";
	responseHeaders += "Content-Type: text/html\r\n";
	responseHeaders += "Content-Length: " + std::to_string(responseBody.size()) + "\r\n";

	std::string response = responseHeaders + "\r\n" + responseBody;

	if (send(response_socket, response.c_str(), response.size(), 0) < 0) {
		std::cout << "send() error on fd " << response_socket << std::endl;
		custom_close(response_socket);
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
	if (send(response_socket, response.c_str(), response.size(), 0) < 0) {
		std::cout << "send() error on fd " << response_socket << std::endl;
		custom_close(response_socket);
	}
}

std::string TestServer::determineCgiScriptPath(const std::string& requestPath) {
	std::cout << "Request path: " << requestPath << std::endl;
	return this->_rootPath  + "/cgi-bin" + requestPath.substr(8);
}

void TestServer::handlePost(HandleRequest &request, int response_socket) {
    std::string path = request.getPath();

	if (std::stoi(request.getHeader("Content-Length")) > _max_body_size)
	{
		std::string filePath = this->_rootPath + "/error_webpage/custom413.html";
		std::ifstream file(filePath);
		if (!file.is_open())
		{
			std::cerr << "Failed to open handlePost\n";
			return;
		}

		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string responseBody = buffer.str();

		std::string responseHeaders = "HTTP/1.1 413 Payload Too Large\r\n";
		responseHeaders += "Content-Type: text/html\r\n";
		responseHeaders += "Content-Length: " + std::to_string(responseBody.size()) + "\r\n";

		std::string response = responseHeaders + "\r\n" + responseBody;

		if (send(response_socket, response.c_str(), response.size(), 0) < 0) {
			std::cout << "send() error on fd " << response_socket << std::endl;
			custom_close(response_socket);
		}
		return;
	}
	

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
		if (send(response_socket, cgiOutput.c_str(), cgiOutput.size(), 0) < 0) {
			std::cout << "send() error on fd " << response_socket << std::endl;
			custom_close(response_socket);
		}
	}

	else {
		std::string filePath = this->_rootPath + "/default_webpages/siteHome.html";
		std::ifstream file(filePath);
		if (!file.is_open())
		{
			std::cerr << "Failed to open handlePost\n";
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
		{
			std::cout << "send() error on fd " << response_socket << std::endl;
			custom_close(response_socket);
		}
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
		custom_close(response_socket);
	}
}

