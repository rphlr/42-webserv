/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:04:58 by rrouille          #+#    #+#             */
/*   Updated: 2024/01/26 14:11:41 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
