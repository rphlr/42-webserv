/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:09:07 by ckarl             #+#    #+#             */
/*   Updated: 2024/05/08 14:04:37 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Config/ParserConfig.hpp"

Parser::Parser(void) : _inServ(false), _inLoc(false), _inErr(false), _currentServer(NULL) {}
Parser::~Parser(void) { _servers.clear(); }
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

	line = line.substr(0, line.find("#"));
	pos = line.find(sign::DOUBLE_DOT);
	if (pos == (int)std::string::npos)
		throw std::runtime_error(INVALID_CONF + "a line is missing the delimitor ':'");
	key = line.substr(0, pos);
	value = line.substr(pos + 1);
	if (_inErr)
		_currentServer->setErrorPage(key, value);
	else if (_inLoc)
		_currentServer->handleLocation(key, value);
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
			(_currentServer->*setValue[i])(value);
			return ;
		}
	}
	throw std::invalid_argument(INVALID_CONF + "unknown element in settings");

}

void	Parser::validServerConfig()
{
	for (vector<Server>::iterator it = _servers.begin(); it != (_servers.end() - 1); it++) {
		for (vector<Server>::iterator it2 = it + 1; it2 != _servers.end(); it2++) {
			if (*it == *it2)
				throw std::runtime_error("Two or more _servers have the same name, port or host");
		}
	}
	for (vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++) {
		if (it->isComplete() == false)
			throw std::runtime_error(INVALID_CONF + "incomplete server config");
	}
}

vector<Server>	Parser::parseFile(string doc)
{
	std::ifstream	inputFile;

	inputFile.open(doc, std::fstream::in);
	if (inputFile.fail() || inputFile.peek() == EOF)
		throw std::runtime_error(FILE_OPENING);

	string line;
	while (std::getline(inputFile, line))
	{
		line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
		if (line.empty() || line.at(0) == '#')
			continue;
		if (line == "server:") {
			_inLoc = false; _inErr = false; _inServ = true;
			_servers.push_back(Server());
			_currentServer = &_servers.back();
			continue;
		}
		if (line == "error_page:") {
			if (_inErr || _inLoc || !_inServ)
				throw std::runtime_error(SECTION_ERR + "error_page in wrong section");
			_inErr = true;
			continue;
		}
		if (line == "location:") {
			if (_inServ) {
				_inLoc = true; _inErr = false;
				_currentServer->addLocationChangePointer();
			}
			else
				throw std::runtime_error(SECTION_ERR + "location outside of server section");
			continue;
		}
		handleLine(line);
	}
	validServerConfig();
	inputFile.close();
	return _servers;
}
