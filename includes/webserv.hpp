/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:04:58 by rrouille          #+#    #+#             */
/*   Updated: 2024/02/19 17:22:38 by ckarl            ###   ########.fr       */
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
#include <sys/socket.h>

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