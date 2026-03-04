/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jferrand <jferrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:22:35 by jferrand          #+#    #+#             */
/*   Updated: 2026/03/04 12:41:33 by tlecuyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

int	main(int argc, char **argv)
{
	Server	ser;

	if (argc != 3)
		return ((std::cout << "Irc error : Bad arguments " << std::endl), 1);
	if (atoi(argv[1]) < 1024 || atoi(argv[1]) > 65535)
		return ((std::cout << "Irc error : Bad arguments " << std::endl), 1);
	std::cout << "---- SERVER ----" << std::endl;
	ser.setPassword(argv[2]);
	ser.setPort(atoi(argv[1]));
	try
	{
		signal(SIGINT, Server::signalHandler);
		signal(SIGQUIT, Server::signalHandler);
		ser.serverInit();
	}
	catch (const std::exception &e)
	{
		ser.closeServer();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
	return (0);
}

std::vector<std::string> split(std::string str, std::string delimiter)
{
	size_t next = 0;
	std::vector<std::string> array;
	while ((next = str.find(delimiter)) != std::string::npos)
	{
		array.push_back(str.substr(0, next));
		str.erase(0, next + delimiter.length());
	}
	array.push_back(str);
	return (array);
}