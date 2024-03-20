
#ifndef BINDING_SOCKET_HPP
# define BINDING_SOCKET_HPP

#include "./SimpleSocket.hpp"
/*
ConnectingSocket, BindingSocket: These are derivative classes of SimpleSocket.
The ConnectingSocket class would take care of establishing a connection with the network.
BindingSocket binds the socket to an address and port,
which allows the socket to listen to incoming connections on that address and port.
*/
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