/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 22:33:57 by ckarl             #+#    #+#             */
/*   Updated: 2024/04/18 17:41:35 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "Errors.hpp"

class Server;

using std::string;
using std::vector;

/*
one or multiple of the following:
	◦ Define a list of accepted HTTP methods for the route.
	◦ Define a HTTP redirection.
	◦ Define a directory or a file from where the file should be searched (for example, if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is /tmp/www/pouic/toto/pouet).
	◦ Turn on or off directory listing.
*/

class Location
{
public:
	Location(void);
	~Location(void);
	Location(const Location &c);
	Location &operator = (const Location &c);

	//create a == overloaded operator

	void	setPath(string &p);
	void	setRedirect(string &r);
	void	setMethods(string &m);
	void	setDirList(string &d);

	string	&getPath(void);
	string	&getRedirect(void);
	std::vector<string>	&getMethods(void);
	bool	&getDirList(void);

	bool	validMethod(string &m);
	bool	checkMethod(string &m);

private:
	string					_path;
	string					_redirect;
	std::vector<string>		_methods;
	bool					_directory_listing;
};

//outstream overload operator for htis class
std::ostream& operator << (std::ostream& os, Location &obj);

#endif
