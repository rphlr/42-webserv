/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:04:58 by rrouille          #+#    #+#             */
/*   Updated: 2024/03/22 19:21:18 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdlib.h>
#include <cerrno>

#include "Sockets/BindingSocket.hpp"
#include "Sockets/ListeningSocket.hpp"
#include "Sockets/SimpleSocket.hpp"
#include "Sockets/ConnectingSocket.hpp"

#include "Server/SimpleServer.hpp"
#include "Server/TestServer.hpp"

#define BUFFER_SIZE 1024

class Webserv
{
	public:
		Webserv(std::string config_file);
		~Webserv(void);
		void	run(void);
	private:
		std::string	_config_file;
};

#endif