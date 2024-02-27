/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckarl <ckarl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:03:09 by ckarl             #+#    #+#             */
/*   Updated: 2024/02/27 20:34:19 by ckarl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <string>
using std::string;

const string FILE_OPENING = "This file could not be opened or is empty";
const string FILE_SYNTAX = "This file contains a syntax error";
const string ERROR_CODE = "This error code does not exist";
const string NO_ERR = "This server does not have any error_pages";
const string INVALID_CONF = "The configuration file is invalid due to: ";
const string SECTION_ERR = "A subsection or setting is outside of the allowed scope: ";
const string UNAVAILABLE_SERV = "This server is unavailable, it does not exist";
const string NO_LOC = "This server does not have any locations";

#endif