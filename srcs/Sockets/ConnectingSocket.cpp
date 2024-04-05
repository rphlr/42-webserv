#include "../../includes/Sockets/ConnectingSocket.hpp"

/* *********************************** *
**  Canonical Form ******************* *
* *********************************** */
ConnectingSocket::ConnectingSocket( int domain, int service, int protocol, int port, u_long interface ) :
	SimpleSocket( domain, service, protocol, port, interface ) {
	std::cout << "ConnectingSocket constructor called\n";
	set_connection(connect_to_network(get_sock(), get_address()));

}

/* *********************************** *
**  Core functionnalities ************ *
* *********************************** */
int ConnectingSocket::connect_to_network(int sock, struct sockaddr_in address) {
	return bind(sock, (struct sockaddr *)&address, sizeof(address));
}

