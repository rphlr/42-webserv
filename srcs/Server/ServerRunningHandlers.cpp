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
		// std::cerr << "Could not open requested error page " << error_code << std::endl;
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
	std::string rootPath = this->_rootPath;
	std::string scriptPath = rootPath + requestPath;
	return scriptPath;
}

#include <sstream>
#include <string>
#include <map>

std::string ServerRunning::handlePostData(const std::string &postData) {
	std::stringstream ss(postData);
	std::string line;

	for (int i = 0; i < 4; i++)
		std::getline(ss, line);
	std::string buffer;
	while (std::getline(ss, line) && line.find("------WebKitFormBoundary") == std::string::npos)
		buffer += line + "\n";
	return buffer;
}

std::string ServerRunning::getFileName(const std::string &postData) {
	std::stringstream ss(postData);
	std::string line;
	std::string filePath;

	for (int i = 0; i < 2; i++)
		std::getline(ss, line);
	
	filePath = line.substr(line.find("filename=") + 10);
	filePath = filePath.substr(0, filePath.find("\""));
	std::cout << RED << "File path: " << filePath << RESET << std::endl;
	return filePath;
}

void ServerRunning::handlePost(HandleRequest &request, int response_socket) {
	std::string path = request.getPath();
	// std::cout << "path: " << path << std::endl;
	try {
		std::stoi(request.getHeader("Content-Length"));
	}
	catch (std::exception &e) {
		handleErrorFilePath(response_socket, 413);
		return;
	}

	if (std::stoi(request.getHeader("Content-Length")) > _max_body_size)
	{
		handleErrorFilePath(response_socket, 413);
		return;
	}
	else if (path.find("/cgi-bin/") != std::string::npos) {
		std::cout << "Handling CGI\n";
		std::map<std::string, std::string> cgiEnv;
		std::cout << "Request method: " << cgiEnv["REQUEST_METHOD"] << std::endl;
		std::string scriptPath = determineCgiScriptPath(path);

		std::string postData = request.getRequest();
		std::string data = handlePostData(postData);
		// save data to temp file
		std::string tempFilePath = "/tmp/" + getFileName(postData);
		std::ofstream tempFile(tempFilePath, std::ios::out | std::ios::binary);
		tempFile << data;
		tempFile.close();

		cgiEnv["REQUEST_METHOD"] = "POST";
		cgiEnv["CONTENT_LENGTH"] = std::to_string(data.size());
		cgiEnv["CONTENT_TYPE"] = request.getHeader("Content-Type");
		cgiEnv["SCRIPT_NAME"] = scriptPath;
		cgiEnv["FILENAME"] = getFileName(postData);
		cgiEnv["SERVER_PROTOCOL"] = request.getProtocol();
		cgiEnv["SERVER_SOFTWARE"] = "webserv";
		cgiEnv["SERVER_PORT"] = std::to_string(_port);
		cgiEnv["REDIRECT_STATUS"] = "200";

		HandleCGI cgiHandler(scriptPath, cgiEnv, data);
		std::cout << "Executing CGI\n";
		std::string cgiOutput = cgiHandler.execute();

		std::cout << "CGI output: " << cgiOutput << std::endl;
		// custom_send(response_socket, cgiOutput.c_str(), cgiOutput.size());
	}
	else {
		std::string homePath = "/default_webpages/siteHome.html";
		handleFilePath(response_socket, homePath);
	}
}

void ServerRunning::handleDelete(HandleRequest &request, int response_socket) {
	std::string path = request.getPath();

	std::string filename = path.substr(path.find("file=") + 5);

	std::cout << "Filename: " << filename << std::endl;
	std::cout << "Path: " << path << std::endl;

	std::string filePath = _rootPath + "/webpages/default_webpages/uploads/" + filename;
	std::cout << "File path: " << filePath << std::endl;

	std::string scriptPath = determineCgiScriptPath("/cgi-bin/deleteFile.php");
	std::map<std::string, std::string> cgiEnv;
	cgiEnv["REQUEST_METHOD"] = "DELETE";
	cgiEnv["SCRIPT_NAME"] = scriptPath;
	cgiEnv["FILENAME"] = filename;
	cgiEnv["SERVER_PROTOCOL"] = request.getProtocol();
	cgiEnv["SERVER_SOFTWARE"] = "webserv";
	cgiEnv["SERVER_PORT"] = std::to_string(_port);
	cgiEnv["REDIRECT_STATUS"] = "200";

	HandleCGI cgiHandler(scriptPath, cgiEnv, "");
	std::string cgiOutput = cgiHandler.execute();
	std::cout << "CGI output: " << cgiOutput << std::endl;


	std::string homePath = "/default_webpages/siteHome.html";
	handleFilePath(response_socket, homePath);
}
