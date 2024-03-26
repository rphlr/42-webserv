#include "../../includes/Server/HandleRequest.hpp"

HandleRequest::HandleRequest( char *incoming_request ) : _request(incoming_request) {
	std::cout << "!!! Incoming request !!!\n";
	std::cout << _request;
}

void HandleRequest::handleRequest() {
	// First line is the method path and protocol
	std::string extracted_request = _request.substr(0, _request.find("\n"));
	std::stringstream iss(extracted_request);
	std::string segment;
	std::vector<std::string> segments;

	while (std::getline(iss, segment, ' ')) {
		segments.push_back(segment);
	}

	for (int i = 0; i < segments.size(); i++) {
		_method = segments[0];
		_path = segments[1];
		_protocol = segments[2];
	}

	std::cout << "!!! Request line parsed !!!\n";
	std::cout << "Method: " << _method << std::endl;
	std::cout << "Path: " << _path << std::endl;
	std::cout << "Protocol: " << _protocol << std::endl;


	// Next lines are the headers
	
	// Last line is the body
	std::cout << "\n\n\n";

}

HandleRequest::~HandleRequest() {}