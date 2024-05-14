#ifndef SIMPLE_SERVER_HPP
# define SIMPLE_SERVER_HPP

#include "../Networking/Networking.hpp"
#include "../Config/Config.hpp"

#include <unistd.h>
#include <stdlib.h>

// class SimpleServer {
// 	public:
// 		SimpleServer( int domain, int service, int protocol, int port, u_long interface, int bklg );
// 		virtual void launch() = 0;
// 		ListeningSocket * get_socket();


// 	private:
// 		ListeningSocket * _socket;
// 		/* *********************************** *
// 		**  Server config ******************** *
// 		* *********************************** */
// 		std::string _server_name;
// 		int _port;
// 		std::string _host;
// 		int _max_body_size;
// 		// std::string _root;
// 		// std::string _default_file;
// 		// std::map<int, std::string> _error_pages;
// 		// std::vector<Location> _locations;
// 		/* *********************************** *
// 		**  Core functionnalities ************ *
// 		* *********************************** */
// 		virtual void accepter() = 0;
// 		virtual void handler() = 0;
// 		virtual void responder() = 0;
// };

class SimpleServer {
	public:
		SimpleServer( Server &server );
		virtual void launch() = 0;
		ListeningSocket * get_socket();


	protected:
		ListeningSocket * _socket;
		/* *********************************** *
		**  Server config ******************** *
		* *********************************** */
		std::string _server_name;
		int _port;
		std::string _host;
		int _max_body_size;
		std::string _rootPath;
		// std::string _root;
		// std::string _default_file;
		// std::map<int, std::string> _error_pages;
		// std::vector<Location> _locations;
		/* *********************************** *
		**  Core functionnalities ************ *
		* *********************************** */
		virtual void accepter() = 0;
		virtual void handler() = 0;
		virtual void responder() = 0;
};

#endif
