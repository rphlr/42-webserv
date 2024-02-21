/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 22:12:22 by ckarl             #+#    #+#             */
/*   Updated: 2024/02/21 22:38:17 by ckarl            ###   ########.fr       */
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



	void	setName(string n);
	void	setPort(int p);
	void	setHost(string h);
	void	setSize(int s);
	void	setRoot(string r);
	void	setErrorPage(int code, string path);

	string	getName(void) const;
	uint32_t	getPort(void) const;
	string	getHost(void) const;
	uint32_t	getSize(void) const;
	string	getRoot(void) const;
	string	getErrorPath(int code) const;

	class ServerAttributionError : public std::exception
	{
	public:
		virtual const char *what(const char *s) const throw();
	};

private:
	string		_server_name;
	uint32_t	_port;
	string		_host;
	uint32_t	_max_body_size;
	string		_root;
	unordered_map<int, const string>	_error_pages;
	//something for locations like unordered map int, location class


};



#endif