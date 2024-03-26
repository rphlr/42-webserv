#include "../../includes/Server/SimpleServer.hpp"

// /* *********************************** *
// **  Canonical Form ******************* *
// * *********************************** */
// SimpleServer::SimpleServer( int domain, int service, int protocol, int port, u_long interface, int bklg ) {
// 	_socket = new ListeningSocket( domain, service, protocol, port, interface, bklg );
// }

/* *********************************** *
**  Core functionnalities ************ *
* *********************************** */
ListeningSocket * SimpleServer::get_socket() {
	return _socket;
}

// /* *********************************** *
// **  Canonical Form ******************* *
// * *********************************** */
SimpleServer::SimpleServer( Server &server ) {
	int domain = AF_INET;
	int service = SOCK_STREAM;
	int protocol = 0;
	int port = server.getPort();
	u_long interface;
	if (!server.getHost().compare("127.0.0.1")){
		std::cout << "Here" << std::endl;
		interface = INADDR_LOOPBACK;
	}
	else {
		std::cout << "There" << std::endl;
		interface = inet_addr(server.getHost().c_str());
	}
	std::cout << "interface: " << server.getHost().c_str() << std::endl;
	int bklg = 10;
	_socket = new ListeningSocket( domain, service, protocol, port, interface, bklg );
}