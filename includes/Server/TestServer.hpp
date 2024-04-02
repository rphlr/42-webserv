#ifndef TEST_SERVER_HPP
#define TEST_SERVER_HPP

#include "../Config/Config.hpp"
#include "./HandleRequest.hpp"
#include "../Networking/Networking.hpp"

#include <map>
#include <functional>
#include <sstream>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <fcntl.h>

class TestServer
{
public:
	TestServer(Server &server);
	void launch();
	void init();
	//need to implement canonical form

private:
	char _buffer[3000];
	int _new_socket;
	int _listen_socket;
	int _max_sockets;
	struct sockaddr_in _address;
	int	_addr_len;
	struct timeval _timeout;
	bool _end_server;
	bool _close_connection;
	fd_set _master_read_fds;
	fd_set _working_read_fds;
	fd_set _write_fds;



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

	void handleRoot(HandleRequest &request);
	void handleCss(HandleRequest &request);
	void handleError(HandleRequest &request);

};

#endif