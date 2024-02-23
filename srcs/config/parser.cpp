/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:09:07 by ckarl             #+#    #+#             */
/*   Updated: 2024/02/23 15:25:59 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"
#include "errors.hpp"
#include "server.hpp"
#include <sstream>
#include <fstream>

Parser::Parser(void) : inServ(false), inErr(false), inLoc(false) {}

Parser::~Parser(void) {}

Parser::Parser(const Parser &c) { (void) c; }

Parser &Parser::operator = (const Parser &c)
{
	(void) c;
	return *this;
}

void	Parser::handleLine(string &line)
{
	//remove whitespace
	line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
	std::cout << line << std::endl;
	//separate by : into two parts
	//check key and check if value typecast ok
	//send to set function and catch error
}

vector<string>	Parser::parseFile(string doc)
{
	//open file
	std::ifstream	inputFile;
	vector<string>	wholeFile;
	Server			ServConf;

	inputFile.open(doc, std::fstream::in);
	if (inputFile.fail())
		throw std::runtime_error(FILE_OPENING);

	//put into vector
	string line;
	string serv = "server:", err = "\terror_page:", loc = "\tlocation:";
	while (std::getline(inputFile, line))
	{
		if (line.empty() || line.at(0) == '#')
			continue;
		if (line == serv) {
			this->inServ = true;
			std::cout << "server section found\n";
			continue;
		}
		if (line == err && this->inServ) {
			if (inErr)
				throw std::runtime_error(SECTION_ERR + "error_page");
			std::cout << "error_page section found\n";
			this->inErr = true;
			continue;
		}
		else
			throw std::runtime_error(SECTION_ERR + "error_page");
		if (line == loc) {
			if (this->inServ) {
				this->inLoc = true; this->inErr = false;
			}
			else
				throw std::runtime_error(SECTION_ERR + "error_page");
			std::cout << "error_page section found\n";
			continue;
		}


		//treat line depending on bool inServ, inErr, inLoc


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
