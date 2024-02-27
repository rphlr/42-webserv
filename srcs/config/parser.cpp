/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:09:07 by ckarl             #+#    #+#             */
/*   Updated: 2024/02/27 18:40:23 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Errors.hpp"
#include "Server.hpp"
#include <sstream>
#include <fstream>

Parser::Parser(void) : inServ(false), inLoc(false), inErr(false), currentServer(nullptr) {}

Parser::~Parser(void) { this->servers.clear(); }

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
	//send to set function - set in *currentSection
	if (this->inErr)
		handleErrorPage(key, value);
	else if (this->inLoc)
		handleLocation(key, value);
	else
		handleSetting(key, value);
}

void	Parser::handleErrorPage(string &key, string &value)
{
	(void)key;
	(void)value;
}

void	Parser::handleLocation(string &key, string &value)
{
	(void)key;
	(void)value;
}

void	Parser::handleSetting(string &key, string &value)
{
	(void)key;
	(void)value;
}

vector<string>	Parser::parseFile(string doc)
{
	std::ifstream	inputFile;
	vector<string>	wholeFile;
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
			this->inLoc = false; this->inErr = false; this->inServ = true;
			//point to next Server
			this->servers.push_back(Server());
			this->currentServer = &servers.back();
			// std::cout << "server section found\n";
			continue;
		}
		if (line == "error_page:") {
			if (inErr || inLoc || !inServ)
				throw std::runtime_error(SECTION_ERR + "error_page in wrong section");
			// std::cout << "error_page section found\n";
			this->inErr = true;
			continue;
		}
		if (line == "location:") {
			if (this->inServ) {
				this->inLoc = true; this->inErr = false;
			}
			else
				throw std::runtime_error(SECTION_ERR + "location outside of server section");
			// std::cout << "location section found\n";
			continue;
		}
		//treat line depending on bool inServ, inErr, inLoc
		handleLine(line);


		//put into vector (TBD if still necessary) OR return a ServerConfig class?
		if (!line.empty())
			wholeFile.push_back(line);
	}


	inputFile.close();
	return wholeFile;
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
