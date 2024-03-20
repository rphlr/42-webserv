/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvaubien <nvaubien@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:03:58 by rrouille          #+#    #+#             */
/*   Updated: 2024/03/20 12:45:50 by nvaubien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/42webserv.hpp"

int	main(int ac, char **av)
{
	Parser	parsing;

	if (ac != 2) {
		std::cout << "Specify a config file as your program parameter" << std::endl;
		return (-1);
	}
	try {
		parsing.parseFile(av[1]);
	}
	catch (std::exception &e)
	{
		std::cout << "\033[1;31mERROR \033[0m" << e.what() << std::endl;
	}

	/* parsing got the private attributes I need for TestServer */

	TestServer t;
	return (0);
}