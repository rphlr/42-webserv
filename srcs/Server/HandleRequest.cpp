#include "../../includes/Server/HandleRequest.hpp"
#include <fstream>
#include <dirent.h>

HandleRequest::HandleRequest( std::string &incoming_request ) {
	_request = incoming_request;
}

std::string HandleRequest::getRequest() const {
	return _request;
}

std::string HandleRequest::getMethod() const {
	return _method;
}

std::string HandleRequest::getPath() const {
	return _path;
}

std::string HandleRequest::getProtocol() const {
	return _protocol;
}

void HandleRequest::setRequest( std::string request ) {
	_request = request;
}

void HandleRequest::setMethod( std::string method ) {
	_method = method;
}

std::string HandleRequest::getBody() const {
	return _body;
}

std::string HandleRequest::listed_files(std::string path)
{
	std::string files;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(path.c_str())) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			if (ent->d_name[0] != '.')
			{
				files += "\t\t\t\t\"" + std::string(ent->d_name) + "\",\n";
			}
		}
		files.pop_back();
		files.pop_back();

		files += "\n";
		closedir(dir);
	}
	else
	{
		perror("could not open directory");
	}
	return files;
}

void HandleRequest::handleRequest() {
	std::string extracted_request = _request.substr(0, _request.find("\n"));

	std::ofstream file("webpages/cgi-bin/files.json");
	file << "{\n";
	file << "\t\"files\": [\n";

	std::string path = "webpages/default_webpages/uploads";
	std::string files = listed_files(path);

	file << files;
	file << "\t\t\t]\n";
	file << "}\n";
	file.close();

	if (extracted_request.find("WebKitFormBoundary") != std::string::npos) {
		// std::cout << "CGI REQUEST" << std::endl;
		std::string boundary = extracted_request.substr(extracted_request.find("WebKitFormBoundary"));
		_method = "POST";

		// Next lines are the headers
		size_t headers_start = _request.find("\n") + 1;
		size_t headers_end = _request.find("\n\n", headers_start);
		std::string headers_str = _request.substr(headers_start, headers_end - headers_start);
		std::stringstream headers_stream(headers_str);
		std::string header_line;
		while (std::getline(headers_stream, header_line)) {
			size_t separator = header_line.find(": ");
			if (separator != std::string::npos) {
				std::string header_name = header_line.substr(0, separator);
				std::string header_value = header_line.substr(separator + 2);
				header_value.erase(std::remove(header_value.begin(), header_value.end(), '\r'), header_value.end());
				_headers[header_name] = header_value;
			}
		}
		_headers["Content-Length"] = std::to_string(_request.size() - headers_end - 2);
		// get header Content-Disposition
		std::string content_disposition = getHeader("Content-Disposition");
		std::string filename = content_disposition.substr(content_disposition.find("filename=") + 10);
		filename = filename.substr(0, filename.find("\""));
		if (filename.find(".py") != std::string::npos || filename.find(".sh") != std::string::npos || filename.find(".pl") != std::string::npos || filename.find(".php") != std::string::npos)
			 _path = "/default_webpages/executeCode.html?file=" + filename;
		 else
			 _path = "/default_webpages/cgi.html?upload=true";
		_protocol = "HTTP/1.1";
	}
	else
	{
		if (extracted_request.find("HTTP") == std::string::npos)
		{
			_method = "GET";
			_path = "/upload";
			return;
		}
		// std::cout << "HTTP REQUEST" << std::endl;
		extracted_request.erase(std::remove(extracted_request.begin(), extracted_request.end(), '\r'), extracted_request.end());
		std::stringstream iss(extracted_request);
		std::string segment;
		std::vector<std::string> segments;

		while (std::getline(iss, segment, ' '))
			segments.push_back(segment);
		if (segments.size() != 3)
			return;
		_method = segments[0];
		_path = segments[1];
		_protocol = segments[2];

		// Next lines are the headers
		size_t headers_start = _request.find("\r\n") + 2;
		size_t headers_end = _request.find("\r\n\r\n", headers_start);
		std::string headers_str = _request.substr(headers_start, headers_end - headers_start);
		std::stringstream headers_stream(headers_str);
		std::string header_line;
		while (std::getline(headers_stream, header_line)) {
			size_t separator = header_line.find(": ");
			if (separator != std::string::npos) {
				std::string header_name = header_line.substr(0, separator);
				std::string header_value = header_line.substr(separator + 2);
				header_value.erase(std::remove(header_value.begin(), header_value.end(), '\r'), header_value.end());
				_headers[header_name] = header_value;
			}
		}
	}
	// Last line is the body
	size_t body_start = _request.find("\r\n\r\n");
	if (body_start != std::string::npos) {
		_body = _request.substr(body_start + 4);
	}

	// handle cookie
	if (_path.find("add_cookie.py") != std::string::npos) {
		std::string key = _path.substr(_path.find("key=") + 4);
		std::string value;
		size_t ampPos = key.find("&");
		if (ampPos != std::string::npos) {
			value = key.substr(ampPos + 6);  // Assume "value=" follows "&" directly
			key = key.substr(0, ampPos);
		}

		// Set cookie if key and value are not empty
		if (!key.empty() && !value.empty()) {
			std::string cookie = key + value;
			_headers["Cookie"] = cookie;
		}
		// std::cout << "cookie: " << _headers["Cookie"] << std::endl;
	}

	// handle get cookie
	if (_path.find("get_cookie.py") != std::string::npos) {
		std::string cookie = getHeader("Cookie");
		_body = cookie;
	}
	// std::cout << MAGENTA;
	// std::cout << "Method: " << _method << std::endl;
	// std::cout << "Path: " << _path << std::endl;
	// // std::cout << "Protocol: " << _protocol << std::endl;
	// // std::cout << "Headers: " << std::endl;
	// // for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it) {
	// // 	std::cout << it->first << ": " << it->second << std::endl;
	// // }
	// std::cout << RESET;

}

HandleRequest::~HandleRequest() {}

std::string HandleRequest::getHeader(const std::string& headerName) const {
    std::map<std::string, std::string>::const_iterator it = _headers.find(headerName);
    if (it != _headers.end())
        return it->second;
    return "";
}
