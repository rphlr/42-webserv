/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 15:44:50 by rrouille          #+#    #+#             */
/*   Updated: 2024/01/26 16:21:44 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

using Handler = std::function<void()>;

void setupRoutes() {
	std::map<std::string, Handler> routes;

	routes["/first"] = [](){ serveWebsite("website/first/"); };

	// Handler pour le site web 2
	routes["/second"] = [](){ serveWebsite("website/second/"); };

	// Handler pour le site web 3
	routes["/third"] = [](){ serveWebsite("website/third/"); };

	// Set routes dans le serveur web
	Webserv::setRoutes(routes);
}

void serveWebsite(const std::string& path) {
	std::cout << "Serving website at " << path << std::endl;
}