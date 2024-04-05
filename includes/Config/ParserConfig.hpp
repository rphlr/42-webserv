/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvaubien <nvaubien@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:21:27 by ckarl             #+#    #+#             */
/*   Updated: 2024/03/20 14:42:30 by nvaubien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "./ServerConfig.hpp"

class Server;

using std::string;
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
	void	handleSetting(string &key, string &value);

	Server *getNthServer(int n);
	//check all servers in the end -> isComplete, double ports, other errors ? double locations (not sure if this is an error)

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
