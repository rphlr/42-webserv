/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:03:58 by rrouille          #+#    #+#             */
/*   Updated: 2024/04/03 15:33:25 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/42webserv.hpp"

void sig_handler(int sig)
{
	throw std::runtime_error("Signal problem");
}

int	main(int ac, char **av)
{
	Parser	parsing;
	std::vector<Server> config_servers;
	std::vector<TestServer> run_servers;


	if (ac != 2) {
		std::cout << "Specify a config file as your program parameter" << std::endl;
		return (-1);
	}
	try {
		signal(SIGPIPE, sig_handler);
		config_servers = parsing.parseFile(av[1]);
		for (std::vector<Server>::iterator it = config_servers.begin(); it != config_servers.end(); it++) {
			TestServer init_new_server(*it);
			run_servers.push_back(init_new_server);
		}
		for (int i = 0; i < run_servers.size(); i++) {
			run_servers[i].launch();
		}
		// TestServer	server(*parsing.getNthServer(0));
	}
	catch (std::exception &e)
	{
		std::cout << "\033[1;31mERROR \033[0m" << e.what() << std::endl;
	}




	return (0);
}