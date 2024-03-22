#ifndef SIMPLE_SERVER_HPP
# define SIMPLE_SERVER_HPP

#include "../Networking.hpp"

class SimpleServer {
	public:
		SimpleServer( int domain, int service, int protocol, int port, u_long interface );
		virtual void launch() = 0;
		ListeningSocket * get_socket();


	private:
		ListeningSocket * _socket;
		virtual void accepter() = 0;
		virtual void handler() = 0;
		virtual void responder() = 0;
};

#endif
