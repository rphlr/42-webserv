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

	void handleGet(HandleRequest &request);
	void handlePost(HandleRequest &request);
	void handleDelete(HandleRequest &request);

	void handleRoot(HandleRequest &request);
	void handleCss(HandleRequest &request);
	void handleForm(HandleRequest &request);
	void handleError(HandleRequest &request);

};

#endif