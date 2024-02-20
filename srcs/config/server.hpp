/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 22:12:22 by ckarl             #+#    #+#             */
/*   Updated: 2024/02/20 23:08:22 by ckarl            ###   ########.fr       */
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
	void	setPort(uint16_t p);
	void	setHost(string &h);
	void	setSize(uint32_t s);
	void	setRoot(string r);
	void	setErrorPage(int code, string path);

	const string	getName(void) const;
	const uint16_t	getPort(void) const;
	const string	getHost(void) const;
	const uint32_t	getSize(void) const;
	const string	getRoot(void) const;
	const string	*getErrorPath(int code);

private:
	const string		_server_name;
	const uint16_t	_port;
	const string		_host;
	const uint32_t	_max_body_size;
	const string		_root;
	unordered_map<int, const string>	_error_pages;
	//something for locations


};



#endif