/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:09:07 by ckarl             #+#    #+#             */
/*   Updated: 2024/02/19 19:21:01 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"
#include "errors.hpp"
#include <sstream>
#include <fstream>

Parser::Parser(void)
{
}

Parser::~Parser(void)
{
}

vector<string>	Parser::ParseFile(string doc)
{
	//open file
	std::ifstream	inputFile;
	vector<string>	wholeFile;

	inputFile.open(doc, std::fstream::in);
	if (inputFile.fail())
		throw std::runtime_error(FILE_OPENING);

	string line;
	while (std::getline(inputFile, line))
		wholeFile.push_back(line);

	//check brackets and double dots
	if (!checkBrackets(wholeFile))
		throw std::runtime_error(FILE_SYNTAX);

	inputFile.close();
	return wholeFile;
}

bool	Parser::checkBrackets(vector<string> wholeFile)
{
	int	curlyBracket = 0;

	for (const auto& str : wholeFile)
	{
		for (const auto& c : str)
		{
			if (c == sign::CURLY_OPEN)
				curlyBracket++;
			if (c == sign::CURLY_CLOSE)
				curlyBracket--;
		}
	}
	if (curlyBracket != 0)
		return false;
	return true;
}
