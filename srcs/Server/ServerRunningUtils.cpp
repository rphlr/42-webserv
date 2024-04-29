#include "../../includes/Server/ServerRunning.hpp"

void ServerRunning::custom_close(int i)
{
	if (close(i) < 0)
		std::cerr << "close() failed with errno " << strerror(errno) << std::endl;
}

void ServerRunning::custom_send(int response_socket, const char *response_str, size_t response_size)
{
	if (send(response_socket, response_str, response_size, 0) < 0){
		// std::cerr << "Could not send to client on fd " << response_socket << ", removing client" << std::endl;
		FD_CLR(response_socket, &_write_fds);
		if (FD_ISSET(response_socket, &_read_fds)) {
			FD_CLR(response_socket, &_master_fds);
			FD_CLR(response_socket, &_read_fds);
			custom_close(response_socket);
		}
	}
}

bool	ServerRunning::pathExists(std::string &path_to_check)
{
	std::ifstream file(_rootPath + path_to_check);
	if (!file.good())
		return false;
	return true;
}

void	ServerRunning::sendResponse(int response_socket, std::string bufferstr, int status_code, std::string content_type)
{
	std::string responseBody = bufferstr;
	std::string responseHeaders = "HTTP/1.1 " + std::to_string(status_code) + " " + _response_code.at(status_code) +"\r\n";
	responseHeaders += "Content-Type: " + content_type + "\r\n";
	responseHeaders += "Content-Length: " + std::to_string(responseBody.size()) + "\r\n";
	std::string fullResponse = responseHeaders + "\r\n" + responseBody;

	custom_send(response_socket, fullResponse.c_str(), fullResponse.size());
}