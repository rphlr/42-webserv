#include "../../includes/Server/TestServer.hpp"
#include "../../includes/Server/HandleRequest.hpp"
#include "../../includes/Server/HandleResponse.hpp"

// TestServer::TestServer() : SimpleServer( AF_INET, SOCK_STREAM, 0, 6545, INADDR_ANY, 10 ) {
// 	launch();
// }

TestServer::TestServer(Server &server) : SimpleServer( server ) {
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
	memset(_buffer, 0, 300 );
	struct sockaddr_in address = get_socket()->get_address();
	int addrlen = sizeof(address);
	_new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t*)&addrlen);
	read( _new_socket, _buffer, 300 );
}

void TestServer::handler() {
	// std::cout << "Handling...\n";
	// std::cout << _buffer << std::endl;
	HandleRequest request(_buffer);
	request.handleRequest();
}

void TestServer::responder() {
	// HandleResponse response;
	std::cout << "Responding...\n";
	const char* response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Salut a tous</h1></body></html>";
	send(_new_socket, response, strlen(response), 0);
	shutdown(_new_socket, SHUT_RDWR);
}

/* TESTING */

// void TestServer::launch() 
// {
// 	fd_set current_sockets, ready_sockets;
// 	FD_ZERO(&current_sockets); // Clears the fd_set

// 	// Add listening socket to the set
// 	FD_SET(get_socket()->get_sock(), &current_sockets);

// 	int max_fd = get_socket()->get_sock();

// 	while (true) 
// 	{
// 		ready_sockets = current_sockets;

// 		if (select(max_fd + 1, &ready_sockets, NULL, NULL, NULL) == -1) 
// 		{
// 			std::cerr << "Select error: " << strerror(errno);
// 			break;
// 		}

// 		// Check each socket for readiness
// 		for(int i = 0; i <= max_fd; i++) 
// 		{
// 			if (FD_ISSET(i, &ready_sockets)) 
// 			{
// 				if (i == max_fd) 
// 				{
// 					// Accept new connection
// 					accepter();
// 				} 
// 				else 
// 				{
// 					handler();

// 					if(strlen(_buffer) !=  0) // Check if there's data in the buffer
// 					{
// 						responder();
// 					}
// 				}
// 			}
// 		}
// 	}
// }
