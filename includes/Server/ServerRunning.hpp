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
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>
#include <dirent.h>

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
	// void	sendCookies(std::string &cookies, int response_socket);

private:
	std::string _rootPath;
	std::string _full_request;
	int _new_socket;
	int _listen_socket;
	int _max_nbr_of_sockets;
	struct sockaddr_in _address;
	int	_addr_len;
	struct timeval _timeout;
	fd_set _master_fds;
	fd_set _read_fds;
	fd_set _write_fds;
	std::map<int, std::string> _cookies;

	int _port;
	std::string _host;
	int _max_body_size;
	std::string _default_file;
	std::map<int, std::string> _error_pages;
	std::vector<Location> _locations;
	std::map<int, std::string> _response_code;

	void receiver(int receive_socket);
	void handler(int response_socket);
	void handleGet(HandleRequest &new_request, int response_socket);
	void handlePost(HandleRequest &new_request, int response_socket);
	void handleDelete(HandleRequest &new_request, int response_socket);

	void handleFilePath(int response_socket, std::string &path);
	void handleErrorFilePath(int response_socket, int error_code);
	void handleCss(int response_socket);

	std::string handlePostData(const std::string &postData);
	std::string getFileName(const std::string &postData);
	std::string handleGetData(const std::string &getData);

	void checkIfRedirectionNeeded(int response_socket, std::string &path_to_check, std::string &method);
	std::string generateDirectoryListing(const std::string& directoryPath);
	bool pathExists(std::string &path_to_check);
	void sendResponse(int response_socket, std::string buffer, int status_code, std::string content_type);
	void sendResponse(int response_socket, std::string buffer, int status_code, std::string content_type, std::string &cookies);
	void custom_send(int response_socket, const char *response_str, size_t response_size);
	void custom_close(int i);

	// get and set cookies
	void addCookie(std::string &cookie);
	std::map<int, std::string> getCookies();

	std::string determineCgiScriptPath(const std::string &path);
};

#endif

