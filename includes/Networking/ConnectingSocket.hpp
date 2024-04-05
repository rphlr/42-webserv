
#ifndef Connecting_SOCKET_HPP
# define Connecting_SOCKET_HPP

#include "./SimpleSocket.hpp"

class ConnectingSocket : public SimpleSocket {
	public:
		// Canonical class
		ConnectingSocket( int domain, int service, int protocol, int port, u_long interface );

		// ConnectingSocket( ConnectingSocket const & src );
		// ConnectingSocket& operator=( ConnectingSocket const & rhs );
		// ~ConnectingSocket();

		// Functions
		int connect_to_network(int sock, struct sockaddr_in address);

};

#endif