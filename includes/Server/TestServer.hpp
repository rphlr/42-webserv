#ifndef TEST_SERVER_HPP
#define TEST_SERVER_HPP

#include "./SimpleServer.hpp"
#include "../Config/Config.hpp"
#include "./HandleRequest.hpp"

#include <map>
#include <functional>
#include <sstream>

// class TestServer : public SimpleServer {
// 	public:
// 		TestServer();
// 		void launch();

// 	private:
// 		char _buffer[3000];
// 		int _new_socket;

// 		void accepter();
// 		void handler();
// 		void responder();

// };

class TestServer : public SimpleServer
{
public:
	TestServer(Server &server);
	void launch();

private:
	char _buffer[3000];
	int _new_socket;

	void accepter();
	void handler();
	void responder();

	HandleRequest _request;

	typedef void (TestServer::*RouteHandler)(HandleRequest &request);
	std::map<std::string, RouteHandler> _routes;

	void handleRoot(HandleRequest &request)
	{
		// 
		std::ifstream file("/home/nate/Workspace/42projects/42-webserv/webpages/default_webpage/siteHome.html");

		if (!file.is_open())
		{
			// handle the error, e.g. by logging it and returning
			std::cerr << "Failed to open file\n";
			return;
		}
		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string responseBody = buffer.str();

		std::string responseHeaders = "HTTP/1.1 200 Ok\r\n";
		responseHeaders += "Content-Type: text/html\r\n";
		responseHeaders += "Content-Length: " + std::to_string(responseBody.size()) + "\r\n";

		std::string response = responseHeaders + "\r\n" + responseBody;

		send(_new_socket, response.c_str(), response.size(), 0);
	}
};

#endif