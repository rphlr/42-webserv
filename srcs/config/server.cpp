/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 22:40:45 by ckarl             #+#    #+#             */
/*   Updated: 2024/02/22 19:05:31 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "errors.hpp"

Server::Server(void)
{
	//  : _server_name(""), _port(0), _host(0), _max_body_size(0), _root("")
	// _all_set.insert(std::make_pair("_sn", false));
	// _all_set.insert(std::make_pair("_p", false));
	// _all_set.insert(std::make_pair("_h", false));
	// _all_set.insert(std::make_pair("_mbs", false));
	// _all_set.insert(std::make_pair("_r", false));
	// _all_set.insert(std::make_pair("_ep", false));
}

Server::~Server(void) {}

Server::Server(const Server &c) : _server_name(c._server_name), _port(c._port),
								_host(c._host), _max_body_size(c._max_body_size),
								_root(c._root), _error_pages(c._error_pages) {}

Server &Server::operator = (const Server &c)
{
	(void)c;
	return *this;
}

void	Server::setName(string n)
{
	if (n.size() < 20 && !n.empty())
		this->_server_name = n;
	else
		throw std::runtime_error(INVALID_CONF + "server_name (too long or empty)");
}

void	Server::setPort(int p)
{
	if (p > 0 && p < 65535)
		this->_port = p;
	else
		throw std::overflow_error(INVALID_CONF + "port");
}

void	Server::setSize(int s)
{
	if (s > 1000 || s < 0)					//check for BUFFER_SIZE (maybe throw error?)
		throw std::runtime_error(INVALID_CONF + "max_body_size");
	else if (!this->_all_set["_mbs"]){
		this->_max_body_size = s;
		this->_all_set["_mbs"] = 1;
	}
}

void	Server::setRoot(string r)
{
	(void)r;
	//check here if path exists or not?
}

void	Server::setErrorPage(int code, string &path)
{
	if (code >= 100 && code < 600)
		this->_error_pages[code] = path;
	else
		throw std::runtime_error(INVALID_CONF + "error_page (code does not exist)");
}

void	Server::setHost(string h) { this->_host = h; }

string	Server::getName(void) const { return this->_server_name; }
uint32_t	Server::getPort(void) const { return this->_port; }
string	Server::getHost(void) const { return this->_host; }
uint32_t	Server::getSize(void) const { return this->_max_body_size; }
string	Server::getRoot(void) const { return this->_root; }
string	Server::getErrorPath(int code) const
{
	if (this->_error_pages.find(code) != this->_error_pages.end())
		return this->_error_pages.at(code);
	throw std::logic_error(ERROR_CODE);
}