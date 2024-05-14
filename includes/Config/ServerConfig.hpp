/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 22:12:22 by ckarl             #+#    #+#             */
/*   Updated: 2024/05/07 08:17:40 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "../Errors.hpp"
#include "./LocationConfig.hpp"

using std::string;
using std::vector;

class Server
{
public:
	Server(void);
	~Server(void);
	Server(const Server &c);
	Server &operator = (const Server &c);

	//create a == overloaded operator
	bool	operator == ( const Server &comp );

	void	setName(string &n);
	void	setPort(string &po);
	void	setHost(string &h);
	void	setSize(string &si);
	void	setRoot(string &r);
	void	setDefFile(string &d);
	void	setErrorPage(string &c, string &path);

	string					&getName(void);
	int						&getPort(void);
	string					&getHost(void);
	int						&getSize(void);
	string					&getRoot(void);
	string					&getDefFile(void);
	Location				*getCurrentLoc(void);
	string					&getErrorPath(int code);
	vector<Location>		&getLocations(void);
	std::map<int, string>	&getErrorPages(void);
	string					&getServerName(void);

	bool	isComplete(void);
	void	addLocationChangePointer(void);
	void	handleLocation(string &key, string &value);

private:
	string		_server_name;
	int			_port;
	string		_host;
	int			_max_body_size;
	string		_root;
	string		_default_file;
	std::map<int, string>	_error_pages;
	std::map<int, string>	_cookies;
	vector<Location> _locations;
	Location	*_currentLoc;
};

//create an output stream overloaded operator <<
std::ostream& operator << (std::ostream& os, Server &obj);

#endif
