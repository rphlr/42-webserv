/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:21:27 by ckarl             #+#    #+#             */
/*   Updated: 2024/02/27 20:56:30 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include "Server.hpp"
#include "Location.hpp"

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

	vector<Server>	parseFile(string doc);
	void	handleLine(string &line);
	void	handleLocation(string &key, string &value);
	void	handleSetting(string &key, string &value);
	// bool	checkBrackets(vector<string> wholeFile);

	//check all servers in the end -> isComplete, double ports, other errors ?

protected:
	bool	inServ;
	bool	inLoc;
	bool	inErr;
	vector<Server> servers;
	Server	*currentServer;


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