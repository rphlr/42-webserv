
#ifndef BINDING_SOCKET_HPP
# define BINDING_SOCKET_HPP

#include "./SimpleSocket.hpp"

class BindingSocket : public SimpleSocket {
	public:
		// Canonical class
		BindingSocket( int domain, int service, int protocol, int port, u_long interface );

		// BindingSocket( BindingSocket const & src );
		// BindingSocket& operator=( BindingSocket const & rhs );
		// ~BindingSocket();

		// Functions
		int connect_to_network(int sock, struct sockaddr_in address);

};

#endif