/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jferrand <jferrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:22:35 by jferrand          #+#    #+#             */
/*   Updated: 2026/02/25 14:49:56 by jferrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

int	main(int argc, char **argv)
{
	if (argc != 3)
		return ((std::cout << "Irc error : Bad arguments " << std::endl), 1);
	// Server ser;
	// std::cout << "---- SERVER ----" << std::endl;
	// try{
	// 	signal(SIGINT, Server::signalHandler); //-> catch the signal (ctrl + c)
	// 	signal(SIGQUIT, Server::signalHandler); //-> catch the signal (ctrl + \)
	// 	ser.serverInit(); //-> initialize the server
	// }
	// catch(const std::exception& e){
	// 	ser.closeFds(); //-> close the file descriptors
	// 	std::cerr << e.what() << std::endl;
	// }
	// std::cout << "The Server Closed!" << std::endl;

	return (0);
}
