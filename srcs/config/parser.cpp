/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:09:07 by ckarl             #+#    #+#             */
/*   Updated: 2024/02/22 19:05:38 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"
#include "errors.hpp"
#include "server.hpp"
#include <sstream>
#include <fstream>

Parser::Parser(void) {}

Parser::~Parser(void) {}

Parser::Parser(const Parser &c) { (void) c; }

Parser &Parser::operator = (const Parser &c)
{
	(void) c;
	return *this;
}

vector<string>	Parser::parseFile(string doc)
{
	//open file
	std::ifstream	inputFile;
	vector<string>	wholeFile;
	Server			ServConf;

	// std::cout << ServConf._all_set["_server_name"] << std::endl;
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
		// line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
		// std::cout << line << std::endl;
		if (!line.compare(serv)) {
			std::cout << "server found\n";
		}
		if (!line.compare(err)) {
			std::cout << "error_page found\n";
		}
		if (!line.compare(loc)) {
			std::cout << "location found\n";
		}

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
