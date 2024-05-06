/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:03:58 by rrouille          #+#    #+#             */
/*   Updated: 2024/05/06 15:07:28 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/42webserv.hpp"

void sig_handler(int sig)
{
	std::cerr << "Connection interrupted by a signal" << std::endl;
}

void	server_running(std::vector<Server> config_servers)
{
	std::vector<ServerRunning> run_servers;

	for (std::vector<Server>::iterator it = config_servers.begin(); it != config_servers.end(); it++) {
			ServerRunning init_new_server(*it);
			run_servers.push_back(init_new_server);
	}
	while(1) {
		for (size_t i = 0; i < run_servers.size(); i++)
			run_servers[i].run();
	}
}

void	print_infos(std::vector<Server> config_servers)
{
	const int totalWidth = 80;
	// std::cout << "There " << (config_servers.size() == 1 ? "is " : "are ") << config_servers.size() << " server" << (config_servers.size() == 1 ? "" : "s") << " in the configuration file." "\n";
	for (std::vector<Server>::iterator it = config_servers.begin(); it != config_servers.end(); it++) {
		int serverIndex = std::distance(config_servers.begin(), it) + 1;
		std::cout << (serverIndex == 1 ? "First" : (serverIndex == 2 ? "Second" : (serverIndex == 3 ? "Third" : std::to_string(serverIndex) + "th"))) << " server configuration:" "\n";
		std::cout << RED "┌" BOLD "SERVER CONFIG" RESET RED "─────────────────────────────────────────────────────────────┐" RESET "\n";
		std::cout << RED "│" RESET << std::left << std::setw(34) << BOLD " Name:" RESET << std::left << std::setw(totalWidth - 32) << it->getServerName() << RED "│" RESET "\n";
		std::cout << RED "│" RESET << std::left << std::setw(34) << BOLD " Host:" RESET << std::left << std::setw(totalWidth - 32) << it->getHost() << RED "│" RESET "\n";
		std::cout << RED "│" RESET << std::left << std::setw(34) << BOLD " Port:" RESET << std::left << std::setw(totalWidth - 32) << it->getPort() << RED "│" RESET "\n";
		std::cout << RED "│" RESET << std::left << std::setw(34) << BOLD " Root:" RESET << std::left << std::setw(totalWidth - 32) << it->getRoot() << RED "│" RESET "\n";
		std::cout << RED "│" RESET << std::left << std::setw(34) << BOLD " Default file:" RESET << std::left << std::setw(totalWidth - 32) << it->getDefFile() << RED "│" RESET "\n";
		std::cout << RED "│" RESET << std::left << std::setw(34) << BOLD " Max body size:" RESET << std::left << std::setw(totalWidth - 32) << it->getSize() << RED "│" RESET "\n";
		std::cout << RED "└──────────────────────────────────────────────────────────────────────────┘" << RESET << std::endl;

		// Print error pages
		std::cout << MAGENTA "┌" BOLD "ERROR PAGES" RESET MAGENTA "───────────────────────────────────────────────────────────────┐" RESET "\n";
		for (std::map<int, std::string>::iterator it2 = it->getErrorPages().begin(); it2 != it->getErrorPages().end(); it2++)
		{
			if (it2->first == -1)
				continue;
			std::cout << MAGENTA "│" RESET " " BOLD << it2->first << std::left << std::setw(25) << ":" RESET << std::left << std::setw(totalWidth - 31) << it2->second << MAGENTA "│" RESET "\n";
		}
		std::cout << MAGENTA "└──────────────────────────────────────────────────────────────────────────┘" RESET << std::endl;

		// Print locations
		bool first = true;
		for (std::vector<Location>::iterator it2 = it->getLocations().begin(); it2 != it->getLocations().end(); it2++) {
			if (first)
				std::cout << BLUE "┌" BOLD "LOCATIONS" RESET BLUE "─────────────────────────────────────────────────────────────────┐" RESET "\n";
			else
				std::cout << BLUE "├──────────────────────────────────────────────────────────────────────────┤" RESET "\n";
			std::cout << BLUE "│" RESET << std::left << std::setw(34) << BOLD " Path:" RESET << std::left << std::setw(totalWidth - 32) << it2->getPath() << BLUE "│" RESET "\n";
			if (it2->getRedirect().size() > 0)
				std::cout << BLUE "│" RESET << std::left << std::setw(34) << BOLD " Redirection:" RESET << std::left << std::setw(totalWidth - 32) << it2->getRedirect() << BLUE "│" RESET "\n";
			std::string methods_str;
			for (std::vector<std::string>::iterator it3 = it2->getMethods().begin(); it3 != it2->getMethods().end(); it3++) {
				if (it3 != it2->getMethods().begin())
					methods_str += ", ";
				methods_str += *it3;
			}
			std::cout << BLUE "│" RESET << std::left << std::setw(34) << BOLD " Methods:" RESET << std::left << std::setw(totalWidth - 32) << methods_str << BLUE "│" RESET "\n";
			std::cout << BLUE "│" RESET << std::left << std::setw(34) << BOLD " Directory Listing:" RESET << std::left << std::setw(totalWidth - 27) << (it2->getDirList() ? GREEN "ON" : RED "OFF") << BLUE "│" RESET "\n";
			first = false;
		}
		std::cout << BLUE "└──────────────────────────────────────────────────────────────────────────┘" RESET << std::endl;
	}
	std::cout << GREEN "┌" BOLD "SERVER" << (config_servers.size() == 1 ? "─" : "S") << " INFO" RESET GREEN "──────────────────────────────────────────────────────────────┐" RESET "\n";
	for (std::vector<Server>::iterator it = config_servers.begin(); it != config_servers.end(); it++) {
		std::string serverInfo = " Server " BOLD + it->getServerName() + RESET " is running on " BOLD + it->getHost() + RESET " and listening on port " BOLD + std::to_string(it->getPort()) + RESET;
		std::cout << GREEN "│" RESET << std::left << std::setw(totalWidth + 18) << serverInfo << GREEN "│" RESET "\n";
	}
	std::cout << GREEN "└──────────────────────────────────────────────────────────────────────────┘" RESET << std::endl;
}

int	main(int ac, char **av)
{
	Parser	parsing;
	std::vector<Server> config_servers;
	std::string	config;
	std::vector<ServerRunning> run_servers;

	if (ac > 2) {
		std::cout << "Usage: ./webserv <config_file>" << std::endl;
		return (-1);
	}
	try {
		signal(SIGPIPE, SIG_IGN);
		config = (ac == 1 ? "configfiles/default.conf" : av[1]);
		config_servers = parsing.parseFile(config);
		print_infos(config_servers);
		server_running(config_servers);
	}
	catch (std::exception &e) {
		std::cout << "\033[1;31mERROR \033[0m" << e.what() << std::endl;
	}

	return (0);
}