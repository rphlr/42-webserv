#ifndef HANDLE_REQUEST_HPP
# define HANDLE_REQUEST_HPP

#include <iostream>
#include <iomanip>
#include <map>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstring>

class HandleRequest {
	public:
		HandleRequest( std::string &incoming_request );
		~HandleRequest();
		void handleRequest();

		void	setRequest( std::string request );
		void	setMethod( std::string method );
		void	setPath( std::string path );
		void	setProtocol( std::string protocol );
		void	setHeaders( std::map<std::string, std::string> headers );
		void	setBody( std::string body );

		std::string getRequest() const;
		std::string getMethod() const;
		std::string getPath() const;
		std::string getProtocol() const;
		std::map<std::string, std::string> getHeaders() const;
        std::string getHeader(const std::string& headerName) const;
        std::string getBody() const;

	private:
		std::string _request;
		std::string _method;
		std::string _path;
		std::string _protocol;
		std::map<std::string, std::string> _headers;
		std::string _body;
};

#endif