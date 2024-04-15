#include "../../includes/Server/ServerRunning.hpp"

void ServerRunning::custom_close(int i)
{
	if (close(i) < 0)
		std::cerr << "close() failed with errno " << strerror(errno) << std::endl;
}

void ServerRunning::custom_send(int response_socket, const char *response_str, size_t response_size)
{
	if (send(response_socket, response_str, response_size, 0) < 0){
		std::cout << "send() error on fd " << response_socket << std::endl;
		FD_CLR(response_socket, &_master_fds);
		FD_CLR(response_socket, &_read_fds);
		custom_close(response_socket);
	}
}

std::string &ServerRunning::get_name()
{
	return _server_name;
}
