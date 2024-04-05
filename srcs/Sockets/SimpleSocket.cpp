#include "../../includes/Sockets/SimpleSocket.hpp"


/* *********************************** *
**  Canonical Form ******************* *
* *********************************** */
SimpleSocket::SimpleSocket( int domain, int service, int protocol, int port, u_long interface) {
	std::cout << "SimpleSocket constructor called\n";
	_sock = socket(domain, service, protocol);
	_address.sin_family = domain;
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr = htonl(interface);
	// Establish socket
	_sock = socket(domain, service, protocol);
	test_connection(_sock);
}

/* *********************************** *
**  Core functionnalities ************ *
* *********************************** */
void SimpleSocket::test_connection(int to_test) {
	if (to_test < 0) {
		std::cerr << "Error: " << strerror(errno) << std::endl;
		exit(1);
	}
	else {
		std::cout << "Connection established\n";
	}
}


/* *********************************** *
**  Getters ************************** *
* *********************************** */
int SimpleSocket::get_sock() {
	return _sock;
}

struct sockaddr_in SimpleSocket::get_address() {
	return _address;
}


/* *********************************** *
**  Setters ************************** *
* *********************************** */
void SimpleSocket::set_connection(int con) {
	_connection = con;
}