/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:09:07 by ckarl             #+#    #+#             */
/*   Updated: 2024/04/16 18:32:37 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Config/ParserConfig.hpp"

Parser::Parser(void) : inServ(false), inLoc(false), inErr(false), currentServer(NULL) {}
Parser::~Parser(void) { servers.clear(); }
Parser::Parser(const Parser &c) { (void) c; }
Parser &Parser::operator = (const Parser &c)
{
	(void) c;
	return *this;
}

void	Parser::handleLine(string &line)
{
	string	key;
	string	value;
	int		pos;

	line = line.substr(0, line.find("#"));
	pos = line.find(sign::DOUBLE_DOT);
	if (pos == (int)std::string::npos)
		throw std::runtime_error(INVALID_CONF + "a line is missing the delimitor ':'");
	key = line.substr(0, pos);
	value = line.substr(pos + 1);
	if (inErr)
		currentServer->setErrorPage(key, value);
	else if (inLoc)
		currentServer->handleLocation(key, value);
	else
		handleSetting(key, value);
}

void	Parser::handleSetting(string &key, string &value)
{
	string	setting[6] = {"server_name", "port", "host", "max_body_size", "root", "default_file"};
	void (Server::*setValue[6])(string &value) = {&Server::setName, &Server::setPort, \
			&Server::setHost, &Server::setSize, &Server::setRoot, &Server::setDefFile};

	for (int i = 0; i < 6; i++) {
		if (key == setting[i]) {
			(currentServer->*setValue[i])(value);
			return ;
		}
	}
	throw std::invalid_argument(INVALID_CONF + "unknown element in settings");

}

vector<Server>	Parser::parseFile(string doc)
{
	std::ifstream	inputFile;

	inputFile.open(doc, std::fstream::in);
	if (inputFile.fail() || inputFile.peek() == EOF)
		throw std::runtime_error(FILE_OPENING);

	string line;
	while (std::getline(inputFile, line))
	{
		line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
		if (line.empty() || line.at(0) == '#')
			continue;
		if (line == "server:") {
			inLoc = false; inErr = false; inServ = true;
			servers.push_back(Server());
			currentServer = &servers.back();
			continue;
		}
		if (line == "error_page:") {
			if (inErr || inLoc || !inServ)
				throw std::runtime_error(SECTION_ERR + "error_page in wrong section");
			inErr = true;
			continue;
		}
		if (line == "location:") {
			if (inServ) {
				inLoc = true; inErr = false;
				currentServer->addLocationChangePointer();
			}
			else
				throw std::runtime_error(SECTION_ERR + "location outside of server section");
			continue;
		}
		handleLine(line);
	}
	for (vector<Server>::iterator it = servers.begin(); it != servers.end() - 1; it++)
	{
		for (vector<Server>::iterator it2 = it + 1; it2 != servers.end(); it2++)
		{
			std::cout << "comparing it == it2: " << (*it == *it2) << std::endl;
			// std::cout << "it: " << *it << " and it2: " << *it2 << std::endl;
			if (*it == *it2)
				throw std::runtime_error("Two or more servers have the same name, port and host");
		}
	}
	inputFile.close();
	return servers;
}

Server *Parser::getNthServer(int n)
{
	if (n < 0 || n >= (int)servers.size())
		throw std::out_of_range(INVALID_CONF + "server index out of range");
	return &servers[n];
}

