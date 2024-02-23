/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:21:27 by ckarl             #+#    #+#             */
/*   Updated: 2024/02/23 14:43:47 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

using std::string;
using std::unordered_map;
using std::vector;

class Parser
{
public:
	Parser(void);
	~Parser(void);
	Parser(const Parser &c);
	Parser &operator = (const Parser &c);

	vector<string>	parseFile(string doc);
	void	handleLine(string &line);
	// bool	checkBrackets(vector<string> wholeFile);

private:
	bool	inServ;
	bool	inLoc;
	bool	inErr;

};

namespace sign {
	const char SPACE = ' ';
	const char HYPHEN = '-';
	const char DOUBLE_DOT = ':';
	const char COMMENT_DELIM = ';';
	const char CURLY_OPEN = '{';
	const char CURLY_CLOSE = '}';
	const char QUOTE = '\"';
	const char BACKSLASH = '/';
	const char OVERRIDE_START = '<';
	const char OVERRIDE_END = '>';
	const char DECIMAL = '.';
	const char COMMA = ',';
}

#endif