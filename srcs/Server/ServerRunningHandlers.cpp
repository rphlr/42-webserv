#include "../../includes/Server/ServerRunning.hpp"

void ServerRunning::handleGet(HandleRequest &request, int response_socket) {
	std::string path = request.getPath();
	std::string method = request.getMethod();
	std::cout << "path in handleGet: " << path << std::endl;

	if (path.size() >= 4 && path.substr(path.size() - 4) == ".css") {
		handleCss(response_socket);
	}
	else {
		checkRedirection(response_socket, path, method);
	}
}

std::string generateDirectoryListing(const std::string& directoryPath)
{
	std::stringstream ss;
	ss << "<html><head><title>Directory Listing</title></head><body><h1>Directory Listing: " << directoryPath << "</h1><ul>";

	// Open the directory
	DIR* dir = opendir(directoryPath.c_str());
	if (dir == NULL) {
		ss << "<p>Error: Unable to open directory</p></body></html>";
		return ss.str();
	}

	// Read directory entries
	struct dirent* entry;
	while ((entry = readdir(dir)) != NULL) {
		std::string name = entry->d_name;
		if (name != "." && name != "..") {
			ss << "<li><a href=\"" << name << "\">" << name << "</a></li>";
		}
	}
	closedir(dir);
	ss << "</ul></body></html>";
	return ss.str();
}

void ServerRunning::checkRedirection(int response_socket, std::string &path_to_check, std::string &method)
{
	if (path_to_check.find(".") != std::string::npos) {
		handleFilePath(response_socket, path_to_check);
		return;
	}
	for (std::vector<Location>::iterator it = _locations.begin(); it != _locations.end(); it++)
	{
		if (path_to_check == (*it).getPath())
		{
			if ((*it).getRedirect().empty())
			{
				std::string inLocDefPath(path_to_check + _default_file);
				if (!pathExists(inLocDefPath))
				{
					if ((*it).getDirList() == true)
					{
						std::cout << "in getdireLIst == true" << std::endl;
						const std::string dirListPath = _rootPath + path_to_check;
						std::string dirListHTML = generateDirectoryListing(dirListPath);
						sendResponse(response_socket, dirListHTML, 200, "text/html");
					}
					else
						handleErrorFilePath(response_socket, 404);
				}
				else
					handleFilePath(response_socket, inLocDefPath);
				return;
			}
			else
			{
				if ((*it).checkMethod(method) == false)
					handleErrorFilePath(response_socket, 405);
				else
					handleFilePath(response_socket, (*it).getRedirect());
				return;
			}
		}
	}
	std::string noLocDefPath(path_to_check + _default_file);
	if (!pathExists(noLocDefPath))
		handleErrorFilePath(response_socket, 404);
	else
		handleFilePath(response_socket, noLocDefPath);
	return;
}

void ServerRunning::handleFilePath(int response_socket, std::string &path)
{
	std::stringstream buffer;
	int status_code;
	std::string filePath = _rootPath + path;
	std::ifstream file(filePath);
	if (!file.good())
	{
		std::cout << "handleFilePath: not found 404" << std::endl;
		status_code = 404;
		filePath = _rootPath + _error_pages.at(404);
		std::ifstream errorFile(filePath);
		buffer << errorFile.rdbuf();
	}
	else {
		std::cout << "handleFilePath: okay 200" << std::endl;
		status_code = 200;
		std::ifstream okayFile(filePath);
		buffer << okayFile.rdbuf();
	}
	sendResponse(response_socket, buffer.str(), status_code, "text/html");
}

void ServerRunning::handleErrorFilePath(int response_socket, int error_code)
{
	if (_error_pages.find(error_code) == _error_pages.end())
	{
		std::cerr << "Unknown error page requested" << std::endl;
		return;
	}
	std::stringstream buffer;
	std::string filePath = _rootPath + _error_pages.at(error_code);
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
