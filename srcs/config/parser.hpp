/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:21:27 by ckarl             #+#    #+#             */
/*   Updated: 2024/02/19 19:01:17 by ckarl            ###   ########.fr       */
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

// Helper struct that contains all information required for a single setting.
struct SingleSetting {
	public:
		string section;
		string key;
		string override;
		string value;
};

class Parser
{
public:
	Parser(void);
	~Parser(void);

	//open document & put everything in a vector
	vector<string>	ParseFile(string doc);

	//check if there are brackets, if yes check if they all close
	//check double dots if no brackets TBD
	bool	checkBrackets(vector<string> wholeFile);
	//strip line if it's a comment


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