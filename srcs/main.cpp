/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:03:58 by rrouille          #+#    #+#             */
/*   Updated: 2024/04/05 17:29:17 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/42webserv.hpp"

void sig_handler(int sig)
{
	(void)sig;
	throw std::runtime_error("Signal problem");
}

int	main(int ac, char **av)
{
	Parser	parsing;
	std::vector<Server> config_servers;
	std::vector<TestServer> run_servers;
	std::string	config;


	if (ac > 2) {
		std::cout << "Usage: ./webserv <config_file>" << std::endl;
		return (-1);
	}
	try {

		signal(SIGPIPE, sig_handler);
		config = (ac == 1 ? "configs/default.conf" : av[1]);
		config_servers = parsing.parseFile(config);
		for (std::vector<Server>::iterator it = config_servers.begin(); it != config_servers.end(); it++) {
			TestServer init_new_server(*it);
			run_servers.push_back(init_new_server);
		}
		while(1) {
			for (size_t i = 0; i < run_servers.size(); i++) {
				run_servers[i].run();
			}
		}
	}
	catch (std::exception &e)
	{
		std::cout << "\033[1;31mERROR \033[0m" << e.what() << std::endl;
	}




	return (0);
}