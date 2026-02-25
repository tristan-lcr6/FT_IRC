/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlecuyer <tlecuyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:22:35 by jferrand          #+#    #+#             */
/*   Updated: 2026/02/25 15:14:08 by tlecuyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

int	main(int argc, char **argv)
{
	Server	ser;

	if (argc != 3)
		return ((std::cout << "Irc error : Bad arguments " << std::endl), 1);
	if (atoi(argv[2]) < 1024 || atoi(argv[2]) > 65535 )
		return ((std::cout << "Irc error : Bad arguments " << std::endl), 1);
	std::cout << "---- SERVER ----" << std::endl;
	ser.setPassword(atoi(argv[2]));
	ser.setPort(argv[1]);
	try
	{
		signal(SIGINT, Server::SignalHandler);
		signal(SIGQUIT, Server::SignalHandler);
		ser.ServerInit();
	}
	catch (const std::exception &e)
	{
		ser.CloseFds();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
	return (0);
}
