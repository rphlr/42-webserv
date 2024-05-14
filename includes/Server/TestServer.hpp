#ifndef TEST_SERVER_HPP
#define TEST_SERVER_HPP

#include "../Config/Config.hpp"
#include "./HandleRequest.hpp"
#include "../Networking/Networking.hpp"

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

class TestServer
{
public:
	TestServer(Server &server);
	void launch();
	ListeningSocket * get_socket();

private:
	char _buffer[3000];
	int _new_socket;
	ListeningSocket * _socket;
		/* *********************************** *
	**  Server config ******************** *
	* *********************************** */
	std::string _server_name;
	int _port;
	std::string _host;
	int _max_body_size;
	// std::string _root;
	// std::string _default_file;
	// std::map<int, std::string> _error_pages;
	// std::vector<Location> _locations;

	void accepter();
	void handler();
	void responder();

	HandleRequest _request;

	typedef void (TestServer::*RouteHandler)(HandleRequest &request);
	std::map<std::string, RouteHandler> _routes;

	void handleGet(HandleRequest &request);
	void handlePost(HandleRequest &request);
	void handleDelete(HandleRequest &request);

	void handleHome(HandleRequest &request);
	void handleCss(HandleRequest &request);
	void handleForm(HandleRequest &request);
	void handleError(HandleRequest &request);
	void handleUpload(HandleRequest &request);

	std::string determineCgiScriptPath(const std::string &path);

};

#endif