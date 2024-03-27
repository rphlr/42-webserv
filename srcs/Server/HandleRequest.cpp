#include "../../includes/Server/HandleRequest.hpp"

HandleRequest::HandleRequest( char *incoming_request ) : _request(incoming_request) {
	std::cout << "\033[0;95m!!! Incoming request !!!\n\033[0m";
	if (_request.empty())
		std::cerr << "Error: request is empty" << std::endl;						//throw error
	std::cout << _request;
}

void HandleRequest::handleRequest() {
	std::stringstream iss(_request);
	std::string line;

	// First line is the method, path and protocol
	std::getline(iss, line);
	std::stringstream firstLine(line);
	std::string segment;
	std::vector<std::string> segments;
	while (std::getline(firstLine, segment, ' '))
		segments.push_back(segment);
	if (segments.size() != 3)
		return;										//throw error
	for (int i = 0; i < segments.size(); i++) {
		_method = segments[0];
		_path = segments[1];
		_protocol = segments[2];
	}

	std::cout << "\n\033[0;96m!!! Request line parsed !!!\n\033[0;95m";
	std::cout << "Method: " << _method << std::endl;
	std::cout << "Path: " << _path << std::endl;
	std::cout << "Protocol: " << _protocol << std::endl;


	//parse request headers
	std::string key, value;
	while (std::getline(iss, line) && !line.empty()) {
		int pos = line.find(':');
		key = line.substr(0, pos);
		value = line.substr(pos + 1);
		_headers.insert(std::make_pair(key, value));
		std::cout << "Line: " << line << std::endl;
	}

	//parse request body (only for POST method, will contain data put into the form on our website)
	if (line.empty())
		std::getline(iss, line);
	while (std::getline(iss, line) && !line.empty()) {

		// treat the information filled into the form, example: username=johndoe&password=secretpassword
		// check for max_body_size, if bigger then error code 404
		std::cout << "Line: " << line << std::endl;

	}




}

HandleRequest::~HandleRequest() {}