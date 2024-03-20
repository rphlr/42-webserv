#ifndef SIMPLE_SOCKET_HPP
# define SIMPLE_SOCKET_HPP

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdlib.h>
#include <cerrno>


/* 
SimpleSocket: It's a class that manages creating a socket.
It initializes the _sock object with socket(domain, service, protocol).
It's a base class for ConnectingSocket, BindingSocket, and ListeningSocket.
It receives messages for a given domain, port, and interface.
*/
class SimpleSocket {
	public:
		// Canonical class
		SimpleSocket( int domain, int service, int protocol, int port, u_long interface);
		// SimpleSocket( SimpleSocket const & src );
		// SimpleSocket& operator=( SimpleSocket const & rhs );
		// ~SimpleSocket();

		// Functions
		virtual int connect_to_network(int sock, struct sockaddr_in address) = 0;
		void test_connection(int to_test);

		// Getters
		struct sockaddr_in get_address();
		int	get_sock();

		// Setters
		void set_connection(int con);
		

	private:
		int _sock;
		int _connection;
		struct sockaddr_in _address;
};

#endif