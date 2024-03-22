#include "../../includes/Server/TestServer.hpp"

TestServer::TestServer() : SimpleServer( AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY ) { // 80 = HTTP port number, INADDR_ANY = all interfaces, 0 = default protocol, SOCK_STREAM = TCP
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
	// char *hello = "Hello from server";
	std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!"; // Mettre ici le contenu du fichier demandé
	// write(_new_socket, hello, strlen(hello));
	write(_new_socket, hello.c_str(), hello.length());
	close(_new_socket);
}