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
	size_t headers_start = _request.find("\n") + 1;
	size_t headers_end = _request.find("\n\n");
	std::string headers_str = _request.substr(headers_start, headers_end - headers_start);
	std::stringstream headers_stream(headers_str);
	std::string header_line;
	while (std::getline(headers_stream, header_line)) {
		size_t separator = header_line.find(": ");
		if (separator != std::string::npos) {
			std::string header_name = header_line.substr(0, separator);
			std::string header_value = header_line.substr(separator + 2);
			_headers[header_name] = header_value;
		}
	}

	// Last line is the body
	if (headers_end != std::string::npos) {
		_body = _request.substr(headers_end + 2);
	}

	std::cout << "!!! Headers parsed !!!\n";
	// Loop over the header the c+98 way
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it) {
		std::cout << it->first << ": " << it->second << std::endl;
	}

	std::cout << "!!! Body !!!\n";
	std::cout << _body << std::endl;
}

HandleRequest::~HandleRequest() {}