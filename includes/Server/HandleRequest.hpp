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

#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BRIGHT_BLACK "\033[90m"
#define BRIGHT_RED "\033[91m"

class HandleRequest {
	public:
		HandleRequest( std::string &incoming_request );
		~HandleRequest();
		void handleRequest(std::string serverRoot);

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