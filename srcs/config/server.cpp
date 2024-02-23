/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 22:40:45 by ckarl             #+#    #+#             */
/*   Updated: 2024/02/23 14:31:15 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "errors.hpp"

Server::Server(void): _server_name(""), _port(-1), _host(""), _max_body_size(-1), _root(""), _default_file("") {}

Server::~Server(void) {}

Server::Server(const Server &c) : _server_name(c._server_name), _port(c._port),
								_host(c._host), _max_body_size(c._max_body_size),
								_root(c._root), _default_file(c._default_file),
								_error_pages(c._error_pages) {}

Server &Server::operator = (const Server &c)
{
	(void)c;
	return *this;	//maybe assign after all
}

void	Server::setName(string n)
{
	if (n.size() < 20 && !n.empty() && this->_server_name.empty())
		this->_server_name = n;
	else
		throw std::runtime_error(INVALID_CONF + "server_name (too long, empty or double)");
}

void	Server::setPort(int p)
{
	if (p > 0 && p < 65535 && this->_port < 0)
		this->_port = p;
	else
		throw std::overflow_error(INVALID_CONF + "port (double or size)");
}

void	Server::setSize(int s)
{
	if (s < 1000 && s > 0 && this->_max_body_size < 0)					//check for BUFFER_SIZE (maybe throw error?)
		this->_max_body_size = s;
	else
		throw std::runtime_error(INVALID_CONF + "max_body_size (double or size)");
}

void	Server::setRoot(string r)
{
	if (this->_root.empty() && !r.empty())
		this->_root = r;
	else
		throw std::runtime_error(INVALID_CONF + "root (double)");
	//check here if path exists or not?
}

void	Server::setErrorPage(int code, string &path)
{
	if (code >= 100 && code < 600)
		this->_error_pages[code] = path;
	else
		throw std::runtime_error(INVALID_CONF + "error_page (code invalid)");
}

void	Server::setHost(string h)
{
	if (this->_host.empty() && !h.empty())
		this->_host = h;
	else
		throw std::runtime_error(INVALID_CONF + "host (double)");

}

void	Server::setDefFile(string d)
{
	if (this->_default_file.empty() && !d.empty())
		this->_default_file = d;
	else
		throw std::runtime_error(INVALID_CONF + "default_file (double)");
}

string	Server::getName(void) const { return this->_server_name; }
int		Server::getPort(void) const { return this->_port; }
string	Server::getHost(void) const { return this->_host; }
int		Server::getSize(void) const { return this->_max_body_size; }
string	Server::getRoot(void) const { return this->_root; }
string	Server::getDefFile(void) const { return this->_default_file; }
string	Server::getErrorPath(int code) const
{
	if (this->_error_pages.find(code) != this->_error_pages.end())
		return this->_error_pages.at(code);
	throw std::logic_error(ERROR_CODE);
}