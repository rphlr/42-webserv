#include "../../includes/Server/SimpleServer.hpp"

// /* *********************************** *
// **  Canonical Form ******************* *
// * *********************************** */
SimpleServer::SimpleServer( int domain, int service, int protocol, int port, u_long interface ) {
	_socket = new ListeningSocket( domain, service, protocol, port, interface);
}

/* *********************************** *
**  Core functionnalities ************ *
* *********************************** */
ListeningSocket * SimpleServer::get_socket() {
	return _socket;
}