#include "../../includes/Server/TestServer.hpp"
#include "../../includes/Server/HandleRequest.hpp"
#include "../../includes/Server/HandleResponse.hpp"
#include "../../includes/Server/HandleCGI.hpp"

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

#include <cstdlib>

TestServer::TestServer(Server &server)
{
	char *cwd = getcwd(NULL, 0);
	this->_rootPath = std::string(cwd) + "/" + server.getRoot();
	free(cwd);
	u_long interface;
	_port = server.getPort();
	_address.sin_family = AF_INET;
	_address.sin_port = htons(_port);
	interface = inet_pton(AF_INET, server.getHost().c_str(), &_address.sin_addr);
	if (interface <= 0)
		std::cout << "loopback host or not valid" << std::endl;
	_addr_len = sizeof(_address);
	_max_body_size = server.getSize();

	// Create a function to setup routes
	_routes["/home"] = &TestServer::handleRoot;
	_routes["/styles.css"] = &TestServer::handleCss;
	_routes["/upload"] = &TestServer::handleUpload;
	_routes["/form"] = &TestServer::handleForm;

	init();
}

void custom_close(int i)
{
	if (close(i) < 0)
		std::cerr << "close() failed with errno " << strerror(errno) << std::endl;
}

void TestServer::init()
{
	int on = 1;

	_listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_listen_socket < 0)
	{
		std::cerr << "initializing server socket failed" << std::endl;
		exit(-1);
	}
	if (setsockopt(_listen_socket, SOL_SOCKET, SO_REUSEADDR,
				   (char *)&on, sizeof(on)) < 0)
	{
		custom_close(_listen_socket);
		std::cerr << "setsockopt" << std::endl;
		exit(-1);
	}
	if (fcntl(_listen_socket, F_SETFL, O_NONBLOCK) < 0)
	{
		custom_close(_listen_socket);
		std::cerr << "fcntl failed" << std::endl;
		exit(-1);
	}
	if (bind(_listen_socket, (sockaddr *)&_address, _addr_len) < 0)
	{
		std::cerr << "binding socket failed" << std::endl;
		custom_close(_listen_socket);
		exit(-1);
	}
	if (listen(_listen_socket, 255) < 0)
	{
		std::cerr << "listen() failed" << std::endl;
		custom_close(_listen_socket);
		exit(-1);
	}
	_new_socket = -1;
	FD_ZERO(&_master_fds);
	FD_ZERO(&_write_fds);
	FD_ZERO(&_read_fds);
	_max_nbr_of_sockets = _listen_socket;
	FD_SET(_listen_socket, &_master_fds);

	_timeout.tv_sec = 0;
	_timeout.tv_usec = 0;
}

void TestServer::run()
{
	// FD_COPY(&_master_fds, &_read_fds);
	memcpy(&_read_fds, &_master_fds, sizeof(_master_fds));

	int select_value = select(_max_nbr_of_sockets + 1, &_read_fds, &_write_fds, NULL, &_timeout);
	if (select_value < 0 || select_value > FD_SETSIZE)
	{
		std::cerr << "select() failed" << std::endl;
		for (int i = 0; i <= _max_nbr_of_sockets; i++)
		{
			if (FD_ISSET(i, &_read_fds) && i != _listen_socket)
			{ // double check if isset master or isset read
				FD_CLR(i, &_read_fds);
				FD_CLR(i, &_master_fds);
				custom_close(i);
			}
		}
		return;
	}
	for (int i = 0; i <= _max_nbr_of_sockets; i++)
	{
		if (FD_ISSET(i, &_read_fds) && i == _listen_socket)
		{
			std::cout << "New client on port: " << _port << std::endl;
			_new_socket = accept(_listen_socket, NULL, NULL);
			if (_new_socket < 0 && errno != EWOULDBLOCK)
			{
				std::cerr << "accept() failed for fd " << i << std::endl;
				return;
			}
			else
			{
				FD_SET(_new_socket, &_master_fds);
				if (_max_nbr_of_sockets < _new_socket)
					_max_nbr_of_sockets = _new_socket;
			}
		}
		if (FD_ISSET(i, &_read_fds) && i != _listen_socket)
		{
			memset(_buffer, 0, sizeof(_buffer));
			int rc = recv(i, _buffer, 3000, 0);
			// std::cout << YELLOW << "Raw request:" << _buffer << RESET << "\n\n";
			if (rc <= 0)
			{
				std::cout << (rc == 0 ? "Client closed connection on fd " : "recv() error for fd ") << i << std::endl;
				FD_CLR(i, &_master_fds);
				FD_CLR(i, &_read_fds);
				custom_close(i);
				while (FD_ISSET(_max_nbr_of_sockets, &_master_fds) == false)
					_max_nbr_of_sockets--;
			}
			else
			{
				handler(i);
				FD_SET(i, &_write_fds);
			}
		}
		if (FD_ISSET(i, &_write_fds))
		{
			std::cout << MAGENTA << "Send to handler: " << i << RESET << "\n\n";
			handler(i);
			FD_CLR(i, &_write_fds);
		}
	}
}

void TestServer::handler(int response_socket)
{
	HandleRequest new_request(_buffer);
	new_request.handleRequest();
	std::string method = new_request.getMethod();

	if (method == "GET")
	{
		handleGet(new_request, response_socket);
	}
	else if (method == "POST")
	{
		// handlePost();
		handlePost(new_request, response_socket);
	}
	else if (method == "DELETE")
	{
		// handleDelete(new_request, response_socket);
		std::cout << "DELETE request\n";
	}
	else
	{
		std::cout << "Unsupported method\n";
		handleNotImplemented(new_request, response_socket);
	}
}

std::string &TestServer::getName()
{
	return _server_name;
}
