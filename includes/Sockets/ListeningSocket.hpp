#ifndef LISTENING_SOCKET_HPP
# define LISTENING_SOCKET_HPP

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdlib.h>
#include <cerrno>

#include "../Sockets/BindingSocket.hpp"

class ListeningSocket : public BindingSocket {
	public:
		// Canonical class
		ListeningSocket( int domain, int service, int protocol, int port, u_long interface);

		void start_listening();

	private:
		int _backlog;
		int _listening;
};

#endif