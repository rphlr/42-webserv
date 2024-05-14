/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 22:34:00 by ckarl             #+#    #+#             */
/*   Updated: 2024/05/06 18:43:08 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Config/LocationConfig.hpp"

Location::Location(void) : _path(""), _redirect(""), _directory_listing(false) {}

Location::~Location(void) {}

Location::Location(const Location &c) : _path(c._path), _redirect(c._redirect),
									 _methods(c._methods), _directory_listing(c._directory_listing) {}

Location &Location::operator = (const Location &c)
{
	if (this != &c) {
		_path = c._path;
		_redirect = c._redirect;
		_directory_listing = c._directory_listing;
		_methods = c._methods;
	}
	return *this;
}

void	Location::setPath(string &p)
{
	if (_path.empty() && !p.empty())
		_path = p;
	else
		throw std::invalid_argument(INVALID_CONF + "location path (empty or double)");
}

void	Location::setRedirect(string &r)
{
	if (_redirect.empty() && !r.empty())
		_redirect = r;
	else
		throw std::invalid_argument(INVALID_CONF + "location redirect (empty or double)");
}

bool	Location::validMethod(string &m)
{
	if (m == "POST" || m == "GET" || m == "HEAD" || m == "CONNECT" || m == "PUT" || m == "DELETE" \
	|| m == "OPTIONS" || m == "TRACE" || m == "PATCH")
		return true;
	return false;
}

void	Location::setMethods(string &m)
{
	//separate string m by ',' to get the diff methods
	//check if valid and not yet set and insert in map
	size_t	pos = 0;
	string	temp;
	while (!m.empty())
	{
		pos = m.find(",");
		if (pos == string::npos)
			temp = m;
		else
			temp = m.substr(0, pos);
		if (std::find(_methods.begin(), _methods.end(), temp)!= _methods.end() || validMethod(temp) == false)
			throw std::invalid_argument(INVALID_CONF + "location method (invalid or double)");
		_methods.push_back(temp);
		if (pos != string::npos)
			m = m.substr(pos + 1);
		else
			m.clear();
	}
}

void	Location::setDirList(string &d)		//check for double listing?
{
	if (!d.empty()) {
		if (d == "on")
			_directory_listing = true;
		else if (d == "off")
			_directory_listing = false;
		else
			throw std::invalid_argument(INVALID_CONF + "location directory_listing (invalid)");
	}
	else
		throw std::invalid_argument(INVALID_CONF + "location directory_listing (empty)");
}

bool	Location::checkMethod(string &m)
{
	for (vector<string>::iterator it = _methods.begin(); it != _methods.end(); it++)
	{
		if (*it == m)
			return true;
	}
	return false;
}

string	&Location::getPath(void) { return _path; }
string	&Location::getRedirect(void) { return _redirect; }
std::vector<string>	&Location::getMethods(void) { return _methods; }
bool	&Location::getDirList(void) { return _directory_listing; }


std::ostream& operator << (std::ostream& os, Location &obj)
{
	if (!obj.getPath().empty())
		std::cout << "path: " << obj.getPath() << std::endl;
	if (!obj.getRedirect().empty())
		std::cout << "redirect: " << obj.getRedirect() << std::endl;
	if (!obj.getMethods().empty()) {
		std::vector<string>	tempMethods = obj.getMethods();
		std::cout << "methods: ";
		for(unsigned int i = 0; i < tempMethods.size(); i++) {
			std::cout << tempMethods[i];
			if (i != tempMethods.size() - 1)
				std::cout << ",";
		}
		std::cout << std::endl;
	}
	std::cout << "directory_listing: " << (obj.getDirList() ? "on" : "off") << std::endl;
	return os;
}
