/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:03:58 by rrouille          #+#    #+#             */
/*   Updated: 2024/03/22 19:21:05 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Parser.hpp"

int	main(int ac, char **av)
{
	Parser	parsing;

	if (ac > 2) {
		std::cout << "Usage: ./webserv <config_file>" << std::endl;
		return (-1);
	}
	try {
		TestServer t;
		std::string	config;
		config = (ac == 1 ? "configs/default.conf" : av[1]);
		parsing.parseFile(config);
	}
	catch (std::exception &e)
	{
		std::cout << "\033[1;31mERROR \033[0m" << e.what() << std::endl;
	}

	// std::string	config_file;

	// if (ac == 1)
	// 	config_file = "config/default.conf";
	// else if (ac != 2)
	// {
	// 	std::cout << "Usage: ./webserv <config_file>" << std::endl;
	// 	return (1);
	// }
	// else
	// 	config_file = av[1];
	// Webserv webserv(config_file);
	// webserv.run();
	return (0);
}