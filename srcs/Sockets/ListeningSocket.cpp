#include "../../includes/Sockets/ListeningSocket.hpp"

// /* *********************************** *
// **  Canonical Form ******************* *
// * *********************************** */
ListeningSocket::ListeningSocket( int domain, int service, int protocol, int port, u_long interface) :
	BindingSocket( domain, service, protocol, port, interface ) {
	std::cout << "ListeningSocket constructor called\n";
	start_listening();
}

/* *********************************** *
**  Core functionnalities ************ *
* *********************************** */
void ListeningSocket::start_listening() {
	_listening = listen(get_sock(), _backlog);
}
