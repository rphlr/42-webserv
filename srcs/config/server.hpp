/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 22:12:22 by ckarl             #+#    #+#             */
/*   Updated: 2024/02/27 17:59:05 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

using std::string;
using std::unordered_map;
using std::vector;

class Server
{
public:
	Server(void);
	~Server(void);
	Server(const Server &c);
	Server &operator = (const Server &c);



	void	setName(string &n);
	void	setPort(int &p);
	void	setHost(string &h);
	void	setSize(int &s);
	void	setRoot(string &r);
	void	setDefFile(string &d);
	void	setErrorPage(int &code, string &path);

	string	&getName(void);
	int		&getPort(void);
	string	&getHost(void);
	int		&getSize(void);
	string	&getRoot(void);
	string	&getDefFile(void);
	string	&getErrorPath(int code);


private:
	std::map<string, bool>	_all_set;
	string		_server_name;
	int			_port;
	string		_host;
	int			_max_body_size;
	string		_root;
	string		_default_file;
	std::map<int, string>	_error_pages;
	//something for locations like unordered map int, location class


};



#endif