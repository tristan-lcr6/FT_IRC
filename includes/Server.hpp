/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jferrand <jferrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:22:56 by jferrand          #+#    #+#             */
/*   Updated: 2026/02/25 12:49:53 by jferrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <arpa/inet.h> //-> for inet_ntoa()
# include <csignal>     //-> for signal()
# include <cstdio>     //-> for signal()
# include <fcntl.h>     //-> for fcntl()
# include <iostream>
# include <cstring>
# include <netinet/in.h> //-> for sockaddr_in
# include <poll.h>       //-> for poll()
# include <sys/socket.h> //-> for socket()
# include <sys/types.h>  //-> for socket()
# include <unistd.h>     //-> for close()
# include <vector>       //-> for vector
# include "Client.hpp"

# define BLACK "\033[90m"
# define RED "\033[91m"
# define GREEN "\033[92m"
# define YELLOW "\033[93m"
# define BLUE "\033[94m"
# define PURPLE "\033[95m"
# define CYAN "\033[96m"
# define END "\033[0m"

class Server //-> class for server
{
	private:
		int Port; //-> server port
		int SerSocketFd; //-> server socket file descriptor
		static bool Signal; //-> static boolean for signal
		std::vector<Client> clients; //-> vector of clients
		std::vector<struct pollfd> fds; //-> vector of pollfd
	public:
		Server(){SerSocketFd = -1;} //-> default constructor

		void ServerInit(); //-> server initialization
		void SerSocket(); //-> server socket creation
		void AcceptNewClient(); //-> accept new client
		void ReceiveNewData(int fd); //-> receive new data from a registered client

		static void SignalHandler(int signum); //-> signal handler
	
		void CloseFds(); //-> close file descriptors
		void ClearClient(int fd); //-> clear clients
};

#endif