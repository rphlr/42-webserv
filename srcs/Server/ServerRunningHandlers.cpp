#include "../../includes/Server/ServerRunning.hpp"

std::string ServerRunning::handleGetData(const std::string &getData) {
	std::string filePath = getData.substr(getData.find("file=") + 5);
	filePath = filePath.substr(0, filePath.find("HTTP/1.1") - 1);
	std::ifstream file(filePath);
	std::stringstream buffer;
	buffer << file.rdbuf();

	return buffer.str();
}

void ServerRunning::handleGet(HandleRequest &request, int response_socket) {
	std::string path = request.getPath();
	std::string method = request.getMethod();

	if (path.size() >= 4 && path.substr(path.size() - 4) == ".css") {
		handleCss(response_socket);
	}
	else if (path.find("login") != std::string::npos) {
		std::ifstream file(_rootPath + "/default_webpages/login.html");
		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string content = buffer.str();

		std::string cookie = request.getHeader("Cookie");
		if (cookie.find("id=") != std::string::npos) {
			cookie = cookie.substr(cookie.find("id=") + 3);
			cookie = cookie.substr(0, cookie.find(";"));
		}
		else
			cookie = "";
		std::string user = cookie;

		// Recherche et remplacement dans la std::string
		const std::string searchText = "<!-- loggedInfos -->";

		std::string replaceText = "";

		if (user == "") {
			replaceText = "<h2>Welcome ! Please signup below</h2>"
						"<form action = \"/login\" method = \"post\">"
							"Username: <input type = \"text\" name = \"username\">  <br />"
							"<br>"
							"Password: <input type = \"password\" name = \"password\" />"
							"<input type = \"submit\" value = \"Submit\" /> <br />"
						"<p>Don't have an account? Click <a href=\"/register\">here</a> to register."
						"</form>";
		}
		else {
			replaceText = "<p>Welcome back " + user + "! You are logged in.</p>";
		}
		size_t pos = content.find(searchText);
		if (pos != std::string::npos)
			content.replace(pos, searchText.length(), replaceText);

		// Envoyer la réponse modifiée
		sendResponse(response_socket, content, 200, "text/html");
	}
	else if (path.find("logout") != std::string::npos) {
		std::ifstream file(_rootPath + "/default_webpages/login.html");
		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string content = buffer.str();

		// Recherche et remplacement dans la std::string
		const std::string searchText = "<!-- loggedInfos -->";
		const std::string replaceText = "<h2>Welcome ! Please signup below</h2>"
						"<form action = \"/login\" method = \"post\">"
							"Username: <input type = \"text\" name = \"username\">  <br />"
							"<br>"
							"Password: <input type = \"password\" name = \"password\" />"
							"<input type = \"submit\" value = \"Submit\" /> <br />"
						"<p>Don't have an account? Click <a href=\"/register\">here</a> to register."
						"</form>";
		size_t pos = content.find(searchText);
		if (pos != std::string::npos)
			content.replace(pos, searchText.length(), replaceText);
		
		// delete login cookie
		std::string cookie = request.getHeader("Cookie");
		if (cookie.find("id=") != std::string::npos) {
			cookie = cookie.substr(cookie.find("id=") + 3);
			// deleteCookie(cookie);
		}

		// Envoyer la réponse modifiée
		sendResponse(response_socket, content, 200, "text/html");
	}
	else if (path.find("add_cookie") != std::string::npos) {
		// Lecture du fichier dans une std::string
		std::ifstream file(_rootPath + "/default_webpages/cgi.html");
		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string content = buffer.str();

		// Recherche et remplacement dans la std::string
		const std::string searchText = "<!-- cookieAddingReplacement -->";
		const std::string replaceText = "<p><b>Cookie added!</b></p>";
		size_t pos = content.find(searchText);
		if (pos != std::string::npos)
			content.replace(pos, searchText.length(), replaceText);

		// set cookies
		std::string cookie = request.getHeader("Cookie");
		addCookie(cookie);

		// Envoyer la réponse modifiée
		sendResponse(response_socket, content, 200, "text/html");
	}
	else if (path.find("get_cookie") != std::string::npos) {
		// Lecture du fichier dans une std::string
		std::string cookie = request.getHeader("Cookie");
		std::ifstream file(_rootPath + "/default_webpages/cgi.html");
		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string content = buffer.str();
		std::string cookieName = request.getPath().substr(request.getPath().find("?") + 1);
		cookieName = cookieName.substr(cookieName.find("=") + 1);

		std::string cookieValue;
		size_t pos = cookie.find(cookieName + "=");
		if (pos != std::string::npos && cookieName != "") {
			cookieValue = cookie.substr(pos + cookieName.size() + 1);
			if (cookieValue.find(";") != std::string::npos)
				cookieValue = cookieValue.substr(0, cookieValue.find(";"));
			else
				cookieValue = cookieValue.substr(0, cookieValue.size());
			cookieValue = "Cookie \"<b>" + cookieName + "</b>\" found with value: \"<b>" + cookieValue + "</b>\"";
		}
		else if (cookieName == "") {
			cookieValue = "";
		}
		else {
			cookieValue = "Cookie \"<b>" + cookieName + "</b>\" not found";
		}

		// Recherche et remplacement dans la std::string
		const std::string searchText = "<!-- cookieGettingReplacement -->";
		const std::string replaceText = "<p>" + cookieValue + "</p>";
		pos = content.find(searchText);
		if (pos != std::string::npos)
			content.replace(pos, searchText.length(), replaceText);

		// Envoyer la réponse modifiée
		sendResponse(response_socket, content, 200, "text/html");
	}
	else if (path.find("?file=") != std::string::npos) {
		// CGI request
		std::map<std::string, std::string> cgiEnv;

		std::string getData = request.getRequest();
		std::string data = handleGetData(getData);
		std::string filename = getData.substr(getData.find("file=") + 5);
		filename = filename.substr(0, filename.find("HTTP/1.1") - 1);
		filename = filename.substr(filename.find_last_of("/") + 1);
		std::string FilePath = _rootPath + "/cgi-bin/" + filename;
		std::ofstream uploadedFile(FilePath, std::ios::out | std::ios::binary);
		uploadedFile << data;
		uploadedFile.close();
		std::string scriptPath = determineCgiScriptPath("default_webpages/executeCode.html?" + filename);

		cgiEnv["REQUEST_METHOD"] = "POST";
		cgiEnv["CONTENT_LENGTH"] = request.getHeader("Content-Length");
		cgiEnv["CONTENT_TYPE"] = request.getHeader("Content-Type");
		cgiEnv["SCRIPT_NAME"] = scriptPath;
		cgiEnv["SERVER_PROTOCOL"] = request.getProtocol();
		cgiEnv["SERVER_SOFTWARE"] = "webserv";
		cgiEnv["SERVER_PORT"] = std::to_string(_port);
		cgiEnv["REDIRECT_STATUS"] = "200";
		cgiEnv["FILENAME"] = filename;

		HandleCGI cgiHandler(scriptPath, cgiEnv, data);
		std::string cgiOutput = cgiHandler.execute(_rootPath);

		// Lecture du fichier dans une std::string
		std::ifstream file(_rootPath + "/default_webpages/executeCode.html");
		std::stringstream buffer;
		buffer << file.rdbuf();

		// Extraction de la chaîne du stringstream
		std::string content = buffer.str();

		// Recherche et remplacement dans la std::string
		const std::string searchText = "<!-- results -->";
		const std::string replaceText = cgiOutput;
		size_t pos = content.find(searchText);

		if (pos != std::string::npos) {
			content.replace(pos, searchText.length(), replaceText);
		}

		// Remettre la chaîne modifiée dans le stringstream
		buffer.str("");
		buffer << content;

		// Envoyer la réponse modifiée
		sendResponse(response_socket, content, 200, "text/html");
	}
	else
		checkIfRedirectionNeeded(response_socket, path, method);
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

std::string ServerRunning::handlePostData(const std::string &postData) {
	std::stringstream ss(postData);
	std::string line;

	for (int i = 0; i < 4; i++)
		std::getline(ss, line);
	std::string buffer;
	while (std::getline(ss, line) && line.find("WebKitFormBoundary") == std::string::npos)
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
	return filePath;
}

void ServerRunning::handlePost(HandleRequest &request, int response_socket) {
	std::string path = request.getPath();
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
	else if (path.find("login") != std::string::npos) {
		// get username and password from post data
		std::string postData = request.getRequest();
		std::string username = postData.substr(postData.find("username=") + 9);
		username = username.substr(0, username.find("&"));
		std::string password = postData.substr(postData.find("password=") + 9);
		password = password.substr(0, password.find("&"));

		// check if user exists
		std::ifstream usersFile(_rootPath + "/default_webpages/users.txt");

		// if file doesn't exist, create it
		if (!usersFile.good()) {
			std::ofstream newUsersFile(_rootPath + "/default_webpages/users.txt");
			newUsersFile.close();
		}
		std::string line;
		bool userExists = false;
		while (std::getline(usersFile, line)) {
			if (line == username + ":" + password)
			{
				userExists = true;
				break;
			}
		}

		if (!userExists) {
			std::ifstream file(_rootPath + "/default_webpages/login.html");
			std::stringstream buffer;
			buffer << file.rdbuf();
			std::string content = buffer.str();

			// Recherche et remplacement dans la std::string
			const std::string searchText = "<!-- loggedInfos -->";

			const std::string replaceText = "<h2 style=\"color:red;\">Invalid username or password</h2>"
						"<form action = \"/login\" method = \"post\">"
							"Username: <input type = \"text\" name = \"username\">  <br />"
							"<br>"
							"Password: <input type = \"password\" name = \"password\" />"
							"<input type = \"submit\" value = \"Submit\" /> <br />"
							"<p>Don't have an account? Click <a href=\"/register\">here</a> to register."
						"</form>";
			size_t pos = content.find(searchText);
			if (pos != std::string::npos)
				content.replace(pos, searchText.length(), replaceText);

			// Envoyer la réponse modifiée
			sendResponse(response_socket, content, 200, "text/html");
		}

		std::ifstream file(_rootPath + "/default_webpages/login.html");
		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string content = buffer.str();

		// Recherche et remplacement dans la std::string
		const std::string searchText = "<!-- loggedInfos -->";

		const std::string replaceText = "<p>Welcome back " + username + "! You are logged in.</p>";
		size_t pos = content.find(searchText);
		if (pos != std::string::npos)
			content.replace(pos, searchText.length(), replaceText);

		// Envoyer la réponse modifiée
		sendResponse(response_socket, content, 200, "text/html");
	}
	else if (path.find("register") != std::string::npos) {
		// get username and password from post data
		std::string postData = request.getRequest();
		std::string username = postData.substr(postData.find("login=") + 6);
		username = username.substr(0, username.find("&"));
		std::string password = postData.substr(postData.find("password=") + 9);
		password = password.substr(0, password.find("&"));

		// check if user exists
		std::ifstream usersFile(_rootPath + "/default_webpages/users.txt");

		// if file doesn't exist, create it
		if (!usersFile.good()) {
			std::ofstream newUsersFile(_rootPath + "/default_webpages/users.txt");
			newUsersFile.close();
		}
		std::string line;
		bool userExists = false;
		while (std::getline(usersFile, line)) {
			if (line == username + ":" + password)
			{
				userExists = true;
				break;
			}
		}
		if (!userExists) {
			std::ofstream usersFile(_rootPath + "/default_webpages/users.txt", std::ios::app);
			usersFile << username + ":" + password + "\n";
			usersFile.close();
		}
		else {
			std::ifstream file(_rootPath + "/default_webpages/login.html");
			std::stringstream buffer;
			buffer << file.rdbuf();
			std::string content = buffer.str();

			// Recherche et remplacement dans la std::string
			const std::string searchText = "<!-- loggedInfos -->";

			const std::string replaceText = "<h2 style=\"color:red;\">User already exists</h2>"
						"<form action = \"/login\" method = \"post\">"
							"Username: <input type = \"text\" name = \"username\">  <br />"
							"<br>"
							"Password: <input type = \"password\" name = \"password\" />"
							"<input type = \"submit\" value = \"Submit\" /> <br />"
							"<p>Don't have an account? Click <a href=\"/register\">here</a> to register."
						"</form>";
			size_t pos = content.find(searchText);
			if (pos != std::string::npos)
				content.replace(pos, searchText.length(), replaceText);

			// Envoyer la réponse modifiée
			sendResponse(response_socket, content, 200, "text/html");
		}
		std::ifstream file(_rootPath + "/default_webpages/login.html");
		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string content = buffer.str();

		// Recherche et remplacement dans la std::string
		const std::string searchText = "<!-- loggedInfos -->";

		const std::string replaceText = "<p>Welcome " + username + "! You are logged in.</p>";
		size_t pos = content.find(searchText);
		if (pos != std::string::npos)
			content.replace(pos, searchText.length(), replaceText);

		std::string cookie = "id=" + username;
		addCookie(cookie);

		// Envoyer la réponse modifiée
		sendResponse(response_socket, content, 200, "text/html");
	}
	else if (path.find("upload=true") != std::string::npos) {
		// upload file
		std::string postData = request.getRequest();
		std::string data = handlePostData(postData);

		std::string FilePath = _rootPath + "/default_webpages/uploads/" + getFileName(postData);
		std::ofstream uploadedFile(FilePath, std::ios::out | std::ios::binary);
		uploadedFile << data;
		uploadedFile.close();

		std::string reloadPath = "/default_webpages/cgi.html";
		handleFilePath(response_socket, reloadPath);
	}
	else if (path.find("?file=") != std::string::npos) {
		// CGI request
		std::map<std::string, std::string> cgiEnv;

		std::string postData = request.getRequest();
		std::string data = handlePostData(postData);
		std::string FilePath = _rootPath + "/cgi-bin/" + getFileName(postData);
		std::ofstream uploadedFile(FilePath, std::ios::out | std::ios::binary);
		uploadedFile << data;
		uploadedFile.close();
		std::string scriptPath = determineCgiScriptPath("default_webpages/executeCode.html?" + getFileName(postData));

		cgiEnv["REQUEST_METHOD"] = "POST";
		cgiEnv["CONTENT_LENGTH"] = request.getHeader("Content-Length");
		cgiEnv["CONTENT_TYPE"] = request.getHeader("Content-Type");
		cgiEnv["SCRIPT_NAME"] = scriptPath;
		cgiEnv["SERVER_PROTOCOL"] = request.getProtocol();
		cgiEnv["SERVER_SOFTWARE"] = "webserv";
		cgiEnv["SERVER_PORT"] = std::to_string(_port);
		cgiEnv["REDIRECT_STATUS"] = "200";
		cgiEnv["FILENAME"] = getFileName(postData);

		HandleCGI cgiHandler(scriptPath, cgiEnv, data);
		std::string cgiOutput = cgiHandler.execute(_rootPath);

		// Lecture du fichier dans une std::string
		std::ifstream file(_rootPath + "/default_webpages/executeCode.html");
		std::stringstream buffer;
		buffer << file.rdbuf();

		// Extraction de la chaîne du stringstream
		std::string content = buffer.str();

		// Recherche et remplacement dans la std::string
		const std::string searchText = "<!-- results -->";
		const std::string replaceText = cgiOutput;
		size_t pos = content.find(searchText);

		if (pos != std::string::npos) {
			content.replace(pos, searchText.length(), replaceText);
		}

		// Remettre la chaîne modifiée dans le stringstream
		buffer.str("");
		buffer << content;

		// Envoyer la réponse modifiée
		sendResponse(response_socket, content, 200, "text/html");
	}
	else {
		std::string homePath = "/index.html";
		handleFilePath(response_socket, homePath);
	}
}

void ServerRunning::handleDelete(HandleRequest &request, int response_socket) {
	// get filename from request
	std::string path = request.getPath();
	std::string filePath = "";
	if (path.find("file=") != std::string::npos) {
		std::string filename = path.substr(path.find("file=") + 5);
		filePath = _rootPath + "/default_webpages/uploads/" + filename;

		std::remove(filePath.c_str());

		std::string reloadPath = "/default_webpages/cgi.html";
		handleFilePath(response_socket, reloadPath);
		return;
	}
	else {
		filePath = _rootPath + path;
	}

	if (filePath.find("/default_webpages/uploads/") == std::string::npos || std::remove(filePath.c_str()) != 0) {
		std::cerr << "Error deleting file" << std::endl;
		handleErrorFilePath(response_socket, 501);
		return;
	}
	else if (path.find("file=") == std::string::npos) {
		std::string homePath = "/index.html";
		handleFilePath(response_socket, homePath);
	}
}
