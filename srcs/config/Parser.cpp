/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:09:07 by ckarl             #+#    #+#             */
/*   Updated: 2024/02/29 17:59:14 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser(void) : inServ(false), inLoc(false), inErr(false), currentServer(nullptr) {}

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

	//separate by ':' into two parts
	pos = line.find(sign::DOUBLE_DOT);
	if (pos == (int)std::string::npos)
		throw std::runtime_error(INVALID_CONF + "a line is missing the delimitor ':'");
	key = line.substr(0, pos);
	value = line.substr(pos + 1, line.npos);
	//send to set function - set in *currentServer
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
	// vector<string>	wholeFile;
	// Server			ServConf;

	//check if file can be opened or is empty
	inputFile.open(doc, std::fstream::in);
	if (inputFile.fail() || inputFile.peek() == EOF)
		throw std::runtime_error(FILE_OPENING);

	string line;
	while (std::getline(inputFile, line))
	{
		//remove whitespace
		line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
		if (line.empty() || line.at(0) == '#')
			continue;
		//determine section
		if (line == "server:") {
			inLoc = false; inErr = false; inServ = true;
			//point to next Server
			servers.push_back(Server());
			currentServer = &servers.back();
			// std::cout << "server section found\n";
			continue;
		}
		if (line == "error_page:") {
			if (inErr || inLoc || !inServ)
				throw std::runtime_error(SECTION_ERR + "error_page in wrong section");
			// std::cout << "error_page section found\n";
			inErr = true;
			continue;
		}
		if (line == "location:") {
			if (inServ) {
				inLoc = true; inErr = false;
				//point to next location inside of same server
				currentServer->addLocationChangePointer();
			}
			else
				throw std::runtime_error(SECTION_ERR + "location outside of server section");
			// std::cout << "location section found\n";
			continue;
		}
		//treat line depending on bool inServ, inErr, inLoc
		handleLine(line);

	}
	// std::cout << *currentServer << std::endl;
	for(unsigned int i = 0; i < servers.size(); i++) {
		std::cout << "server:" << servers[i] << std::endl;
	}


	inputFile.close();
	return servers;
}



// bool	Parser::checkBrackets(vector<string> wholeFile)
// {
// 	int	curlyBracket = 0;

// 	for (vector<std::string>::const_iterator it = wholeFile.begin(); it != wholeFile.end(); ++it) {
// 			const string& str = *it;
// 			for (string::const_iterator cit = str.begin(); cit != str.end(); ++cit) {
// 				char c = *cit;
// 				if (c == sign::CURLY_OPEN)
// 					curlyBracket++;
// 				if (c == sign::CURLY_CLOSE)
// 					curlyBracket--;
// 	}
// 	}
// 	return curlyBracket == 0;
// }
