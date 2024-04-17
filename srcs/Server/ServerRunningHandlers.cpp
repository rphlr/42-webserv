#include "../../includes/Server/ServerRunning.hpp"

void ServerRunning::handleNotImplemented(HandleRequest &new_request, int response_socket)
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

	custom_send(response_socket, response.c_str(), response.size());
}

void ServerRunning::handleGet(HandleRequest &request, int response_socket) {
	std::string path = request.getPath();

	if (path.substr(path.size() - 4) == ".css") {
		handleCss(response_socket);
	}
	else {
		// determine if request is directory or file
		// if directory -> determine which file in another function
		handleFilePath(response_socket, path);
	}
}

void ServerRunning::handleFilePath(int response_socket, std::string path)
{
	std::string fullResponse;
	std::stringstream buffer;
	int status_code;
	std::string filePath = _rootPath + path;
	std::ifstream file(filePath);
	if (!file.good() || file.fail())
	{
		status_code = 404;
		filePath = _rootPath + _error_pages.at(404);
		std::ifstream errorFile(filePath);
		buffer << errorFile.rdbuf();
	}
	else {
		status_code = 200;
		std::ifstream okayFile(filePath);
		buffer << okayFile.rdbuf();
	}
	std::string responseBody = buffer.str();
	std::string responseHeaders = "HTTP/1.1 " + std::to_string(status_code) + " " + _response_code.at(status_code) +"\r\n";
	responseHeaders += "Content-Type: text/html\r\n";
	responseHeaders += "Content-Length: " + std::to_string(responseBody.size()) + "\r\n";
	fullResponse = responseHeaders + "\r\n" + responseBody;

	// std::cout << "Response: " << fullResponse << std::endl;

	custom_send(response_socket, fullResponse.c_str(), fullResponse.size());
}

// void ServerRunning::handleUpload(int response_socket) {
// 	std::string filePath = this->_rootPath + "/default_webpages/siteUpDownload.html";
// 	std::cout << "Root path: " << filePath << "\n\n";
// 	std::ifstream file(filePath);
// 	if (!file.is_open())
// 	{
// 		std::cerr << "Failed to open handleUpload\n";
// 		return;
// 	}
// 	std::stringstream buffer;
// 	buffer << file.rdbuf();
// 	std::string responseBody = buffer.str();

// 	std::string responseHeaders = "HTTP/1.1 200 Ok\r\n";
// 	responseHeaders += "Content-Type: text/html\r\n";
// 	responseHeaders += "Content-Length: " + std::to_string(responseBody.size()) + "\r\n";

// 	std::string response = responseHeaders + "\r\n" + responseBody;

// 	// std::cout << "Response: " << response << std::endl;
// 	custom_send(response_socket, response.c_str(), response.size());
// }

std::string ServerRunning::determineCgiScriptPath(const std::string& requestPath) {
	std::cout << "Request path: " << requestPath << std::endl;
	return this->_rootPath  + "/cgi-bin" + requestPath.substr(8);
}

void ServerRunning::handlePost(HandleRequest &request, int response_socket) {
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

		custom_send(response_socket, response.c_str(), response.size());
		return;
	}
	else if (path.substr(0, 9) == "/cgi-bin/") {
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
		custom_send(response_socket, cgiOutput.c_str(), cgiOutput.size());
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

		custom_send(response_socket, response.c_str(), response.size());
	}
}

void ServerRunning::handleDelete(HandleRequest &request, int response_socket) {
	(void) request;
	(void) response_socket;
	// Implement the DELETE request
	// send(_new_socket, response.c_str(), response.size(), 0);
	return;
}

// void ServerRunning::handleRoot(int response_socket)
// {
// 	std::string filePath = this->_rootPath + "/default_webpages/siteHome.html";
// 	// std::cout << "Root path: " << filePath << "\n\n";
// 	std::ifstream file(filePath);
// 	if (!file.is_open())
// 	{
// 		// handle the error, e.g. by logging it and returning
// 		std::cerr << "Failed to open file handle-root\n";
// 		return;
// 	}
// 	std::stringstream buffer;
// 	buffer << file.rdbuf();
// 	std::string responseBody = buffer.str();

// 	std::string responseHeaders = "HTTP/1.1 200 Ok\r\n";
// 	responseHeaders += "Content-Type: text/html\r\n";
// 	responseHeaders += "Content-Length: " + std::to_string(responseBody.size()) + "\r\n";

// 	std::string response = responseHeaders + "\r\n" + responseBody;

// 	custom_send(response_socket, response.c_str(), response.size());
// }

void ServerRunning::handleCss(int response_socket)
{
	std::string filePath = this->_rootPath + "/default_webpages/styles.css";
	std::ifstream file(filePath);

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

	custom_send(response_socket, response.c_str(), response.size());
}

// void ServerRunning::handleForm(int response_socket)
// {
// 	std::string filePath = this->_rootPath + "/default_webpages/siteForm.html";
// 	std::ifstream file(filePath);

// 	if (!file.is_open())
// 	{
// 		// handle the error, e.g. by logging it and returning
// 		std::cerr << "Failed to open handleForm\n";
// 		return;
// 	}
// 	std::stringstream buffer;
// 	buffer << file.rdbuf();
// 	std::string responseBody = buffer.str();

// 	std::string responseHeaders = "HTTP/1.1 200 Ok\r\n";
// 	responseHeaders += "Content-Type: text/html\r\n";
// 	responseHeaders += "Content-Length: " + std::to_string(responseBody.size()) + "\r\n";

// 	std::string response = responseHeaders + "\r\n" + responseBody;

// 	custom_send(response_socket, response.c_str(), response.size());
// }

// void ServerRunning::handleError(int response_socket)
// {
// 	std::string filePath = this->_rootPath + "/error_webpage/custom404.html";
// 	// std::cout << "Root path: " << filePath << std::endl;
// 	std::ifstream file(filePath);
// 	if (!file.is_open())
// 	{
// 		std::cerr << "Failed to open handleError\n";
// 		return;
// 	}
// 	std::stringstream buffer;
// 	buffer << file.rdbuf();
// 	std::string responseBody = buffer.str();

// 	std::string responseHeaders = "HTTP/1.1 404 Not Found\r\n";
// 	responseHeaders += "Content-Type: text/html\r\n";
// 	responseHeaders += "Content-Length: " + std::to_string(responseBody.size()) + "\r\n";

// 	std::string response = responseHeaders + "\r\n" + responseBody;

// 	custom_send(response_socket, response.c_str(), response.size());
// }
