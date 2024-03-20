#ifndef TEST_SERVER_HPP
# define TEST_SERVER_HPP

#include "./SimpleServer.hpp"
#include "../Config/Config.hpp"

// class TestServer : public SimpleServer {
// 	public:
// 		TestServer();
// 		void launch();
		

// 	private:
// 		char _buffer[3000];
// 		int _new_socket;

// 		void accepter();
// 		void handler();
// 		void responder();

// };

class TestServer : public SimpleServer {
	public:
		TestServer( Server &server );
		void launch();
		

	private:
		char _buffer[3000];
		int _new_socket;

		void accepter();
		void handler();
		void responder();

};

#endif