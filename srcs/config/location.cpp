/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 22:34:00 by ckarl             #+#    #+#             */
/*   Updated: 2024/02/28 15:02:28 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(void) : _path(""), _redirect(""), _directory_listing(false) {}
Location::~Location(void) {}

Location::Location(const Location &c) : _path(c._path), _redirect(c._redirect),
									_directory_listing(c._directory_listing), _methods(c._methods){}

Location &Location::operator = (const Location &c)
{
	if (this != &c) {
		_path = c._path;
		_redirect = c._redirect;
		_directory_listing = c._directory_listing;
		_methods = c._methods;
	}
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

void	Location::setMethods(string &m)
{
	//
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

string	&Location::getPath(void) { return _path; }
string	&Location::getRedirect(void) { return _redirect; }
std::map<string, bool>	&Location::getMethods(void) { return _methods; }
bool	&Location::getDirList(void) { return _directory_listing; }



std::ostream& operator << (std::ostream& os, Location &obj)
{
	if (!obj.getPath().empty())
		std::cout << "path: " << obj.getPath() << std::endl;
	//so on for the other variables
}
