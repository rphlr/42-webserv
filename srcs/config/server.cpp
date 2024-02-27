/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 22:40:45 by ckarl             #+#    #+#             */
/*   Updated: 2024/02/27 21:05:31 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void): _server_name(""), _port(-1), _host(""), _max_body_size(-1),
					_root(""), _default_file(""), _currentLoc(nullptr) {}

Server::~Server(void) { _locations.clear(); }

Server::Server(const Server &c) : _server_name(c._server_name), _port(c._port),
								_host(c._host), _max_body_size(c._max_body_size),
								_root(c._root), _default_file(c._default_file),
								_error_pages(c._error_pages), _currentLoc(c._currentLoc) {}

Server &Server::operator = (const Server &c)
{
	(void)c;
	return *this;							//maybe assign after all
}

void	Server::setName(string &n)
{
	if (n.size() < 20 && !n.empty() && _server_name.empty())
		_server_name = n;
	else
		throw std::invalid_argument(INVALID_CONF + "server_name (too long, empty or double)");
}

void	Server::setPort(string &po)
{
	int p = std::stoi(po);
	if (p > 0 && p < 65535 && _port < 0)
		_port = p;
	else
		throw std::invalid_argument(INVALID_CONF + "port (double or size)");
}

void	Server::setSize(string &si)
{
	int s = std::stoi(si);
	if (s < 1000 && s >= 0 && _max_body_size < 0)					//check for BUFFER_SIZE (maybe throw error?)
		_max_body_size = s;
	else
		throw std::invalid_argument(INVALID_CONF + "max_body_size (double or size)");
}

void	Server::setRoot(string &r)
{
	if (_root.empty() && !r.empty())
		_root = r;
	else
		throw std::invalid_argument(INVALID_CONF + "root (double)");
	//check here if path exists or not?
}

void	Server::setErrorPage(string &c, string &path)
{
	int code = stoi(c);
	if (code >= 100 && code < 600 && _error_pages.find(code) == _error_pages.end())
		_error_pages[code] = path;
	else
		throw std::invalid_argument(INVALID_CONF + "error_page (invalid or double)");
}

void	Server::setHost(string &h)
{
	if (_host.empty() && !h.empty())
		_host = h;
	else
		throw std::invalid_argument(INVALID_CONF + "host (double)");

}

void	Server::setDefFile(string &d)
{
	if (_default_file.empty() && !d.empty())
		_default_file = d;
	else
		throw std::runtime_error(INVALID_CONF + "default_file (double)");
}

void	Server::addLocationChangePointer(void)
{
	_locations.push_back(Location());
	_currentLoc = &_locations.back();
}

string	&Server::getName(void) { return _server_name; }
int		&Server::getPort(void) { return _port; }
string	&Server::getHost(void) { return _host; }
int		&Server::getSize(void) { return _max_body_size; }
string	&Server::getRoot(void) { return _root; }
string	&Server::getDefFile(void) { return _default_file; }
Location	*Server::getCurrentLoc(void) { return _currentLoc; }
string	&Server::getErrorPath(int code)
{
	if (_error_pages.find(code) != _error_pages.end())
		return _error_pages.at(code);
	throw std::logic_error(ERROR_CODE);
}

std::map<int, string>	Server::getErrorPages(void)
{
	if (!_error_pages.empty())
		return _error_pages;
	throw std::runtime_error(NO_ERR);
}

vector<Location>	Server::getLocations(void)
{
	if (!_locations.empty())
		return _locations;
	throw std::runtime_error(NO_LOC);
}

bool	Server::isComplete(void)
{
	if (_server_name.empty() || _port < 0 || _host.empty() || _max_body_size < 0 \
		|| _root.empty() || _default_file.empty() || _error_pages.empty() \
		|| _locations.empty())
		return false;
	else
		return true;
}