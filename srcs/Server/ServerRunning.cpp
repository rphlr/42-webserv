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
		_default_file = t._default_file;
		_error_pages = t._error_pages;
		_response_code = t._response_code;
		_locations = t._locations;
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
	_default_file = server.getDefFile();
	_error_pages = server.getErrorPages();
	_locations = server.getLocations();
	_response_code.insert(std::make_pair(200, "Ok"));
	_response_code.insert(std::make_pair(404, "Not Found"));
	_response_code.insert(std::make_pair(405, "Method Not Allowed"));
	_response_code.insert(std::make_pair(413, "Content Too Large"));
	_response_code.insert(std::make_pair(418, "I'm a Teapot"));
	_response_code.insert(std::make_pair(501, "Not Implemented"));
	_response_code.insert(std::make_pair(502, "Bad Gateway"));

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
		std::cerr << RED << "select() failed" << RESET << std::endl;
		for (int i = 0; i <= _max_nbr_of_sockets; i++)
		{
			if (FD_ISSET(i, &_write_fds) && i != _listen_socket)
				FD_CLR(i, &_write_fds);
			if (FD_ISSET(i, &_read_fds) && i != _listen_socket)
			{
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
			_new_socket = accept(_listen_socket, NULL, NULL);
			if (_new_socket < 0 && errno != EWOULDBLOCK)
			{
				std::cerr << RED << "accept() failed for fd " << i << RESET << std::endl;
				return;
			}
			else
			{
				FD_SET(_new_socket, &_master_fds);
				if (_max_nbr_of_sockets < _new_socket)
					_max_nbr_of_sockets = _new_socket;
			}
			std::cout << BLUE << "New client on port: " << _port << " with fd " << _new_socket << RESET << std::endl;
		}
		if (FD_ISSET(i, &_read_fds) && i != _listen_socket)
		{
			receiver(i);
		}
		if (FD_ISSET(i, &_write_fds))
		{
			std::cout << MAGENTA << "Sending to client on fd " << i << RESET << "\n\n";
			handler(i);
		}
	}
}

//determine if chunked or unchunked by checking for transfer encoding: chunked
void	ServerRunning::receiver(int receive_socket)
{
	std::string full_request;
	int bytes_recv = 0;
	char	tmp_buffer[3000];

	for (int i = 0; i < 1000; i++) {
		memset(tmp_buffer, 0, sizeof(tmp_buffer));
		bytes_recv = recv(receive_socket, tmp_buffer, 3000, 0);
		if (bytes_recv < 0)
		{
			std::cerr << "Could not read from client on fd " << receive_socket << ", removing client" << std::endl;
			FD_CLR(receive_socket, &_master_fds);
			FD_CLR(receive_socket, &_read_fds);
			if (FD_ISSET(receive_socket, &_write_fds))
				FD_CLR(receive_socket, &_write_fds);
			custom_close(receive_socket);
			while (FD_ISSET(_max_nbr_of_sockets, &_master_fds) == false)
				_max_nbr_of_sockets--;
			return ;
		}
		full_request.append(tmp_buffer, bytes_recv);
		if (i == 0 && full_request.find("Transfer-Encoding: chunked") == std::string::npos)
			break ;
		if (full_request.find("\r\n0\r\n\r\n") != std::string::npos)
			break ;
	}
	memset(_buffer, 0, sizeof(_buffer));
	strcpy(_buffer, full_request.c_str());
	FD_SET(receive_socket, &_write_fds);
}

void ServerRunning::handler(int response_socket) {
	HandleRequest new_request(_buffer);
	new_request.handleRequest();
	std::string method = new_request.getMethod();

	if (method == "GET")
	{
		std::cout << "GET method" << std::endl;
		handleGet(new_request, response_socket);
	}
	else if (method == "POST")
	{
		std::cout << "POST method" << std::endl;
		handlePost(new_request, response_socket);
	}
	else if (method == "DELETE")
	{
		// handleDelete(new_request, response_socket);
		std::cout << "DELETE method" << std::endl;
	}
	else
	{
		if (method.empty())
			std::cout << "Unsupported method (it's empty) " << std::endl;
		handleErrorFilePath(response_socket, 501);
	}
}
