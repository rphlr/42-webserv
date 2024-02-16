/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:03:58 by rrouille          #+#    #+#             */
/*   Updated: 2024/02/16 08:48:02 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

int	main(int ac, char **av)
{
	std::string	config_file;

	if (ac == 1)
		config_file = "config/default.conf";
	else if (ac != 2)
	{
		std::cout << "Usage: ./webserv <config_file>" << std::endl;
		return (1);
	}
	else
		config_file = av[1];
	Webserv webserv(config_file);
	webserv.run();
	return (0);
}