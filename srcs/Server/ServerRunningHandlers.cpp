#include "../../includes/Server/ServerRunning.hpp"

void ServerRunning::handleGet(HandleRequest &request, int response_socket) {
	std::string path = request.getPath();
	std::string method = request.getMethod();

	if (path.size() >= 4 && path.substr(path.size() - 4) == ".css") {
		handleCss(response_socket);
	}
	else {
		checkIfRedirectionNeeded(response_socket, path, method);
	}
}

void ServerRunning::handleFilePath(int response_socket, std::string &path)
{
	std::stringstream buffer;
	int status_code;
	std::string filePath = _rootPath + path;
	std::ifstream file(filePath);
	if (!file.good())
	{
		status_code = 404;
		filePath = _rootPath + _error_pages.at(-1);
		std::ifstream errorFile(filePath);
		buffer << errorFile.rdbuf();
	}
	else {
		status_code = 200;
		std::ifstream okayFile(filePath);
		buffer << okayFile.rdbuf();
	}
	sendResponse(response_socket, buffer.str(), status_code, "text/html");
}

void ServerRunning::handleErrorFilePath(int response_socket, int error_code)
{
	std::stringstream buffer;
	std::string filePath;
	if (_error_pages.find(error_code) == _error_pages.end())
		filePath = _rootPath + _error_pages.at(-1);
	else
		filePath = _rootPath + _error_pages.at(error_code);
	std::ifstream errorFile(filePath);
	if (!errorFile.good())
	{
		std::cerr << "Could not open requested error page " << error_code << std::endl;
		return ;
	}
	buffer << errorFile.rdbuf();
	sendResponse(response_socket, buffer.str(), error_code, "text/html");
}

void ServerRunning::handleCss(int response_socket)
{
	std::string filePath = this->_rootPath + "/default_webpages/styles.css";
	std::ifstream file(filePath);

	if (!file.is_open())
	{
		std::cerr << "Failed to open handleCss\n";
		return;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	sendResponse(response_socket, buffer.str(), 200, "text/css");
}

std::string ServerRunning::determineCgiScriptPath(const std::string& requestPath) {
	std::cout << "Request path: " << requestPath << std::endl;
	return this->_rootPath  + "/cgi-bin" + requestPath.substr(8);
}

void ServerRunning::handlePost(HandleRequest &request, int response_socket) {
	std::string path = request.getPath();

	if (std::stoi(request.getHeader("Content-Length")) > _max_body_size)
	{
		handleErrorFilePath(response_socket, 413);
		return;
	}
	else if (path.find("/cgi-bin/") != std::string::npos) {
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
		std::string homePath = "/default_webpages/siteHome.html";
		handleFilePath(response_socket, homePath);
	}
}

void ServerRunning::handleDelete(HandleRequest &request, int response_socket) {
	(void) request;
	(void) response_socket;
	// Implement the DELETE request
	// send(_new_socket, response.c_str(), response.size(), 0);
	return;
}
