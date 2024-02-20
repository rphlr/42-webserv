/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 22:40:45 by ckarl             #+#    #+#             */
/*   Updated: 2024/02/20 23:14:14 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server(void) : _server_name("default") {}

~Server::Server(void) {}

Server::Server(const Server &c)
{
	this->_server_name = c._server_name;
	this->_port = c._port;
	this->_host = c._host;
	this->_max_body_size = c._max_body_size;
	this->_root = c._root;
	this->_error_page = c._error_page;
}

Server &Server::operator = (const Server &c)
{
	if (*this != c)
	{
		this->_server_name = c._server_name;
		this->_port = c._port;
		this->_host = c._host;
		this->_max_body_size = c._max_body_size;
		this->_root = c._root;
		this->_error_page = c._error_page;
	}
	return *this;
}

void	Server::setName(string n)
{
	if (n.size() < 20)
		this->_server_name = n;								//account for error case here
}

void	Server::setPort(int p)
{
	if (p > 0 || p < 65535)
		this->_port = p;								//account for error case here
}
void	Server::setHost(string &h) const {this->_host = h;}

void	Server::setSize(uint32_t s)
{
	if (s > 1000)					//check for BUFFER_SIZE (maybe throw error?)
		this->_max_body_size = 1000;
	else
		this->max_body_size = s;
}

void	Server::setRoot(string r)
{
	//check here if path exists or not?
}

void	Server::setErrorPage(int code, string path)
{
	if (code >= 100 && code < 600)
		this->_error_pages[code] = path;					//account for error case here
}




string	Server::getName(void);
uint16_t	Server::getPort(void);
string	Server::getHost(void);
uint32_t	Server::getSize(void);
string	Server::getRoot(void);
string	*Server::getErrorPath(int code);