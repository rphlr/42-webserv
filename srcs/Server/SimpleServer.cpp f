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
// SimpleServer::SimpleServer( Server &server ) {
// 	int domain = AF_INET;
// 	int service = SOCK_STREAM;
// 	int protocol = 0;
// 	int port = server.getPort();
// 	u_long interface;
// 	if (!server.getHost().compare("127.0.0.1")){
// 		interface = INADDR_LOOPBACK;
// 	}
// 	else {
// 		interface = inet_addr(server.getHost().c_str());
// 	}
// 	int bklg = 10;

// 	_socket = new ListeningSocket( domain, service, protocol, port, interface, bklg );
// }

SimpleServer::SimpleServer( Server &server ) {
	int domain = AF_INET;
	int service = SOCK_STREAM;
	int protocol = 0;
	int port = server.getPort();
	u_long interface;
	if (!server.getHost().compare("127.0.0.1")){
		interface = INADDR_LOOPBACK;
	}
	else {
		interface = inet_addr(server.getHost().c_str());
	}
	int bklg = 10;
	char *cwd = getcwd(NULL, 0);
	this->_rootPath = std::string(cwd) + "/" + server.getRoot();
	free(cwd);
	
	_socket = new ListeningSocket( domain, service, protocol, port, interface, bklg );
}