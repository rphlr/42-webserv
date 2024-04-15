#include "../../includes/Server/ServerRunning.hpp"

ServerRunning::~ServerRunning() {}
ServerRunning::ServerRunning(const ServerRunning &t)
{
	*this = t;
}

ServerRunning &ServerRunning::operator = (const ServerRunning &t)
{
	if (this != &t)
	{
		_rootPath = t._rootPath;
		memcpy(_buffer, t._buffer, sizeof(t._buffer));
		_new_socket = t._new_socket;
		_listen_socket = t._listen_socket;
		_max_nbr_of_sockets = t._max_nbr_of_sockets;
		_address = t._address;
		_addr_len = t._addr_len;
		_timeout = t._timeout;
		_master_fds = t._master_fds;
		_read_fds = t. _read_fds;
		_write_fds = t._write_fds;
		_server_name = t._server_name;
		_port = t._port;
		_host = t._host;
		_max_body_size = t._max_body_size;
	}
	return *this;
}

ServerRunning::ServerRunning( Server &server ) {
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
	_routes["/home"] = &ServerRunning::handleRoot;
	_routes["/styles.css"] = &ServerRunning::handleCss;
	_routes["/upload"] = &ServerRunning::handleUpload;
	_routes["/form"] = &ServerRunning::handleForm;

	init();
}

void ServerRunning::init()
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

void ServerRunning::run() {
	FD_COPY(&_master_fds, &_read_fds);

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
		// std::cout << "in run loop" << std::endl;
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

void ServerRunning::handler(int response_socket) {
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
