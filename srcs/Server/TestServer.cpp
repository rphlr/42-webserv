#include "../../includes/Server/TestServer.hpp"

// TestServer::TestServer() : SimpleServer( AF_INET, SOCK_STREAM, 0, 6545, INADDR_ANY, 10 ) {
// 	launch();
// }

TestServer::TestServer() : SimpleServer( AF_INET, SOCK_STREAM, 0, 6545, INADDR_ANY, 1 ) {
	launch();
}

void TestServer::launch() {
	while (true) {
		std::cout << "Waiting for a connection...\n";
		accepter();
		handler();
		responder();
		std::cout << "Done\n";
	}
}

void TestServer::accepter() {
	std::cout << "Accepting...\n";
	memset(_buffer, 0, 3000);
	struct sockaddr_in address = get_socket()->get_address();
	int addrlen = sizeof(address);
	std::cout << get_socket()->get_sock() << std::endl;
	_new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t*)&addrlen);
	read( _new_socket, _buffer, 3000 );
}

void TestServer::handler() {
	std::cout << "Handling...\n";
	std::cout << _buffer << std::endl;
}

void TestServer::responder() {
	std::cout << "Responding...\n";
	char *hello = "Hello from server";
	write(_new_socket, hello, strlen(hello));
	close(_new_socket);
}