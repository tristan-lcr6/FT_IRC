/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlecuyer <tlecuyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:41:46 by jferrand          #+#    #+#             */
/*   Updated: 2026/02/25 13:07:28 by tlecuyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::Signal = false;

void Server::ClearClient(int fd)
{
	for (size_t i = 0; i < fds.size(); i++)
	{
		if (fds[i].fd == fd)
		{
			fds.erase(fds.begin() + i);
			break ;
		}
	}
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].GetFd() == fd)
		{
			clients.erase(clients.begin() + i);
			break ;
		}
	}
}

void Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::Signal = true;
}

void Server::CloseFds()
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		std::cout << RED << "Client <" << clients[i].GetFd() << "> Disconnected" << END << std::endl;
		close(clients[i].GetFd());
	}
	if (SerSocketFd != -1)
	{
		std::cout << RED << "Server <" << SerSocketFd << "> Disconnected" << END << std::endl;
		close(SerSocketFd);
	}
}

void Server::SerSocket()
{
	struct sockaddr_in	add;
	struct pollfd		NewPoll;
	int					en;

	add.sin_family = AF_INET; // ipv4
	add.sin_port = htons(this->Port); // convert the port to network byte order (big endian)
	add.sin_addr.s_addr = INADDR_ANY; // set the address to any local machine address
	SerSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (SerSocketFd == -1)
		throw(std::runtime_error("failed to create socket"));
	en = 1;
	if (setsockopt(SerSocketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		throw(std::runtime_error("failed to set option (SO_REUSEADDR) on socket"));
	if (fcntl(SerSocketFd, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("failed to set option (O_NONBLOCK) on socket"));
	if (bind(SerSocketFd, (struct sockaddr *)&add, sizeof(add)) == -1)
		throw(std::runtime_error("failed to bind socket"));
	if (listen(SerSocketFd, SOMAXCONN) == -1)
		throw(std::runtime_error("listen() failed"));
	NewPoll.fd = SerSocketFd;
	NewPoll.events = POLLIN;
	NewPoll.revents = 0;
	fds.push_back(NewPoll);
}

void Server::ServerInit()
{
	this->Port = 4444;
	SerSocket();
	std::cout << GREEN << "Server <" << SerSocketFd << "> Connected" << END << std::endl;
	std::cout << "Waiting to accept a connection...\n";
	while (Server::Signal == false)
	{
		if ((poll(&fds[0], fds.size(), -1) == -1) && Server::Signal == false)
			throw(std::runtime_error("poll() failed"));
		for (size_t i = 0; i < fds.size(); i++)
		{
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == SerSocketFd)
					AcceptNewClient();
				else
					ReceiveNewData(fds[i].fd);
			}
		}
	}
	CloseFds();
}

void Server::AcceptNewClient()
{
	struct sockaddr_in	cliadd;
	struct pollfd		NewPoll;
	socklen_t			len;

	Client cli;
	len = sizeof(cliadd);
	int incofd = accept(SerSocketFd, (sockaddr *)&(cliadd), &len);
	if (incofd == -1)
	{
		std::cout << "accept() failed" << std::endl;
		return ;
	}
	if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cout << "fcntl() failed" << std::endl;
		return ;
	}
	NewPoll.fd = incofd;
	NewPoll.events = POLLIN;
	NewPoll.revents = 0;
	cli.SetFd(incofd);
	cli.SetIp(inet_ntoa((cliadd.sin_addr)));
	clients.push_back(cli); 
	fds.push_back(NewPoll);
	std::cout << GREEN << "Client <" << incofd << "> Connected" << END << std::endl;
}

void Server::ReceiveNewData(int fd)
{
	char buff[1024]; //-> buffer for the received data
	std::memset(buff, 0, sizeof(buff)); //-> clear the buffer
	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1, 0); //-> receive the data
	if (bytes <= 0)
	{
		std::cout << RED << "Client <" << fd << "> Disconnected" << END << std::endl;
		ClearClient(fd);
		close(fd);
	}
	else
	{ 
		buff[bytes] = '\0';
		std::cout << YELLOW << "Client <" << fd << "> Data: " << END << buff;
		// here you can add your code to process the received data: parse, check, authenticate, handle the command, etc...
	}
}
