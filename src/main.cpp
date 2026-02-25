/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jferrand <jferrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:22:35 by jferrand          #+#    #+#             */
/*   Updated: 2026/02/25 15:50:01 by jferrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

int	main(int argc, char **argv)
{
	Server	ser;

	if (argc != 3)
		return ((std::cout << "Irc error : Bad arguments " << std::endl), 1);
	if (atoi(argv[1]) < 1024 || atoi(argv[1]) > 65535 )
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
		ser.closeFds();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
	return (0);
}
