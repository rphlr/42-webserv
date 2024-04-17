#ifndef SERVERRUNNING_HPP
#define SERVERRUNNING_HPP

#include "../../includes/Config/Config.hpp"
#include "../../includes/Server/HandleRequest.hpp"
#include "../../includes/Server/HandleCGI.hpp"

#include <map>
#include <functional>
#include <sstream>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>

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

class ServerRunning
{
public:
	ServerRunning(Server &server);
	~ServerRunning();
	ServerRunning(const ServerRunning &t);
	ServerRunning &operator = (const ServerRunning &t);

	void run();
	void init();
	std::string &get_name();

private:
	std::string _rootPath;
	char _buffer[3000];
	int _new_socket;
	int _listen_socket;
	int _max_nbr_of_sockets;
	struct sockaddr_in _address;
	int	_addr_len;
	struct timeval _timeout;
	fd_set _master_fds;
	fd_set _read_fds;
	fd_set _write_fds;

	int	get_socket();
	void custom_close(int i);
	void custom_send(int response_socket, const char *response_str, size_t response_size);


	std::string _server_name;
	int _port;
	std::string _host;
	int _max_body_size;
	std::string _root;
	std::string _default_file;
	std::map<int, std::string> _error_pages;
	std::map<int, std::string> _response_code;
	std::vector<Location> _locations;

	void handler(int response_socket);
	// HandleRequest _request;

	typedef void (ServerRunning::*RouteHandler)(int response_socket);
	std::map<std::string, RouteHandler> _routes;

	void handleGet(HandleRequest &new_request, int response_socket);
	void handlePost(HandleRequest &new_request, int response_socket);
	void handleDelete(HandleRequest &new_request, int response_socket);

	void handleFilePath(int response_socket, std::string path);
	// void handleRoot(int response_socket);
	void handleCss(int response_socket);
	// void handleError(int response_socket);
	// void handleForm(int response_socket);
	// void handleUpload(int response_socket);
	// void handleNotImplemented(HandleRequest &new_request, int response_socket);

	std::string determineCgiScriptPath(const std::string &path);
};

#endif

