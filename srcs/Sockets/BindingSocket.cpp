#include "../../includes/Sockets/BindingSocket.hpp"

/* *********************************** *
**  Canonical Form ******************* *
* *********************************** */
BindingSocket::BindingSocket( int domain, int service, int protocol, int port, u_long interface ) :
	SimpleSocket( domain, service, protocol, port, interface ) {
	std::cout << "BindingSocket constructor called\n";
	set_connection(connect_to_network(get_sock(), get_address()));
}

/* *********************************** *
**  Core functionnalities ************ *
* *********************************** */
int BindingSocket::connect_to_network(int sock, struct sockaddr_in address) {
	return bind(sock, (struct sockaddr *)&address, sizeof(address));
}