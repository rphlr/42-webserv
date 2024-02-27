/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 14:06:43 by ckarl             #+#    #+#             */
/*   Updated: 2024/02/27 18:00:25 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config(void) : totalServ(0) {}
Config::~Config(void) { ConfigServers.clear(); }
Config::Config(const Config &c) { (void) c; }

Config &Config::operator = (const Config &c)
{
	(void) c;
	return *this;
}

void	Config::addServer(Server &s)
{
	this->ConfigServers.push_back(s);
	this->totalServ++;
}

int		Config::getTotalServ(void) { return this->totalServ; }

Server	&Config::getServer(int i)
{
	if (i < 0 || (unsigned int)i >= this->ConfigServers.size())
		throw std::runtime_error(UNAVAILABLE_SERV);
	return this->ConfigServers.at(i);
}
