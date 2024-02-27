/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 14:03:12 by ckarl             #+#    #+#             */
/*   Updated: 2024/02/27 14:50:36 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include "Errors.hpp"
#include "Server.hpp"
#include "Location.hpp"

using std::string;
using std::unordered_map;
using std::vector;

class Config
{
public:
	Config(void);
	~Config(void);
	Config(const Config &c);
	Config &operator = (const Config &c);

	//check if diff servers have same port (and other errors TBD)
	void	addServer(Server &s);
	Server	&getServer(int i);
	int		getTotalServ(void);


private:
	vector<Server>	ConfigServers;
	int				totalServ;

};


#endif