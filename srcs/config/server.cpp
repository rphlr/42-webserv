/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 22:40:45 by ckarl             #+#    #+#             */
/*   Updated: 2024/02/21 13:20:13 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "errors.hpp"

Server::Server(void) {}

Server::~Server(void) {}

Server::Server(const Server &c) : _server_name(c._server_name),
								_port(c._port),
								_host(c._host),
								_max_body_size(c._max_body_size),
								_root(c._root),
								_error_pages(c._error_pages) {}

Server &Server::operator = (const Server &c)
{
	(void)c;
	return *this;
}

const char	*Server::ServerAttributionError::what(string s) const throw() { return s; }

void	Server::setName(string n)
{
	if (n.size() < 20 && !n.empty())
		this->_server_name = n;
	else
		throw Server::ServerAttributionError("Invalid server_name (too long or empty)");
}

void	Server::setPort(int p)
{
	if (p > 0 || p < 65535)
		this->_port = p;
	else
		throw std::overflow_error("Invalid port");
}

void	Server::setSize(int s)
{
	if (s > 1000 || s < 0)					//check for BUFFER_SIZE (maybe throw error?)
		throw Server::ServerAttributionError("Invalid max_body_size (exceeding buffer)");
	else
		this->_max_body_size = s;
}

void	Server::setRoot(string r)
{
	//check here if path exists or not?
}

void	Server::setErrorPage(int code, string path)
{
	if (code >= 100 && code < 600)
		this->_error_pages[code] = path;
	else
		throw Server::ServerAttributionError("Invalid error_page attribution (code does not exist)");
}

void	Server::setHost(string h) { this->_host = h; }

string	Server::getName(void) const { return this->_server_name };
uint16_t	Server::getPort(void) const { return this->_port; }
string	Server::getHost(void) const { return this->_host; }
uint32_t	Server::getSize(void) const { return this->_max_body_size; }
string	Server::getRoot(void) const { return this->_root; }
string	Server::getErrorPath(int code) const
{
	if (this->_error_pages.find(code) != this->_error_pages.end())
		return this->_error_pages[code];
	throw std::logic_error(ERROR_CODE);
}