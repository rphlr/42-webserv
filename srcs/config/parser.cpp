/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:09:07 by ckarl             #+#    #+#             */
/*   Updated: 2024/02/21 11:57:45 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"
#include "errors.hpp"
#include <sstream>
#include <fstream>

Parser::Parser(void)
{}

Parser::~Parser(void)
{}

Parser::Parser(const Parser &c)
{
	(void) c;
}

Parser &Parser::operator = (const Parser &c)
{
	(void) c;
	return *this;
}

vector<string>	Parser::ParseFile(string doc)
{
	//open file
	std::ifstream	inputFile;
	vector<string>	wholeFile;

	inputFile.open(doc, std::fstream::in);
	if (inputFile.fail())
		throw std::runtime_error(FILE_OPENING);

	//put into vector
	string line;
	while (std::getline(inputFile, line))
	{
		if (!line.empty())
			wholeFile.push_back(line);
	}
	//check brackets
	// if (!checkBrackets(wholeFile))
	// 	throw std::runtime_error(FILE_SYNTAX);

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
