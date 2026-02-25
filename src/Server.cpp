/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlecuyer <tlecuyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:41:46 by jferrand          #+#    #+#             */
/*   Updated: 2026/02/25 17:45:10 by tlecuyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::_signal = false;

// Default constructor
Server::Server(void) : _port(-1), _serSocketFd(-1), _password("")
{
	// std::cout << "Default constructor called" << std::endl;
	return ;
}

// Copy constructor
Server::Server(const Server &other) : _port(other._port),
	_serSocketFd(other._serSocketFd), _clients(other._clients), _fds(other._fds)
{
	// std::cout << "Copy constructor called" << std::endl;
	return ;
}

// Assignment operator overload
Server &Server::operator=(const Server &other)
{
	// std::cout << "Assignment operator called" << std::endl;
	this->_port = other._port;
	this->_serSocketFd = other._serSocketFd;
	this->_clients = other._clients;
	this->_fds = other._fds;
	return (*this);
}

// Destructor
Server::~Server(void)
{
	// std::cout << "Destructor called" << std::endl;
	return ;
}

void Server::clearClient(int fd)
{
	for (size_t i = 0; i < this->_fds.size(); i++)
	{
		if (this->_fds[i].fd == fd)
		{
			this->_fds.erase(this->_fds.begin() + i);
			break ;
		}
	}
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i].getFd() == fd)
		{
			this->_clients.erase(this->_clients.begin() + i);
			break ;
		}
	}
}

void Server::signalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::_signal = true;
}

void Server::closeFds()
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i].getFd() != -1)
		{
			std::cout << RED << "Client <" << this->_clients[i].getFd() << "> Disconnected" << END << std::endl;
			close(this->_clients[i].getFd());
			this->_clients[i].setFd(-1);
		}
	}
	if (this->_serSocketFd != -1)
	{
		std::cout << RED << "Server <" << this->_serSocketFd << "> Disconnected" << END << std::endl;
		close(this->_serSocketFd);
	}
}

void Server::serSocket()
{
	struct sockaddr_in	add;
	struct pollfd		NewPoll;
	int					en;

	add.sin_family = AF_INET; // ipv4
	add.sin_port = htons(this->_port);
	// convert the port to network byte order (big endian)
	add.sin_addr.s_addr = INADDR_ANY;
	// set the address to any local machine address
	this->_serSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serSocketFd == -1)
		throw(std::runtime_error("failed to create socket"));
	en = 1;
	if (setsockopt(this->_serSocketFd, SOL_SOCKET, SO_REUSEADDR, &en,
			sizeof(en)) == -1)
		throw(std::runtime_error("failed to set option (SO_REUSEADDR) on socket"));
	if (fcntl(this->_serSocketFd, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("failed to set option (O_NONBLOCK) on socket"));
	if (bind(this->_serSocketFd, (struct sockaddr *)&add, sizeof(add)) == -1)
		throw(std::runtime_error("failed to bind socket"));
	if (listen(this->_serSocketFd, SOMAXCONN) == -1)
		throw(std::runtime_error("listen() failed"));
	NewPoll.fd = this->_serSocketFd;
	NewPoll.events = POLLIN;
	NewPoll.revents = 0;
	this->_fds.push_back(NewPoll);
}

void Server::serverInit()
{
	serSocket();
	std::cout << GREEN << "Server <" << this->_serSocketFd << "> Connected" << END << std::endl;
	std::cout << "Waiting to accept a connection...\n";
	while (Server::_signal == false)
	{
		if ((poll(&this->_fds[0], this->_fds.size(), -1) == -1)
			&& Server::_signal == false)
			throw(std::runtime_error("poll() failed"));
		for (size_t i = 0; i < this->_fds.size(); i++)
		{
			if (this->_fds[i].revents & POLLIN)
			{
				if (this->_fds[i].fd == this->_serSocketFd)
					acceptNewClient();
				else
					receiveNewData(this->_fds[i].fd);
			}
		}
	}
	closeFds();
}

void Server::acceptNewClient()
{
	struct sockaddr_in	cliadd;
	struct pollfd		NewPoll;
	socklen_t			len;
	Client				cli;
	int					incofd;

	len = sizeof(cliadd);
	incofd = accept(this->_serSocketFd, (sockaddr *)&(cliadd), &len);
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
	cli.setFd(incofd);
	cli.setIp(inet_ntoa(cliadd.sin_addr));
	this->_clients.push_back(cli);
	this->_fds.push_back(NewPoll);
	std::cout << GREEN << "Client <" << incofd << "> Connected" << END << std::endl;
}

void Server::receiveNewData(int fd)
{
	Client	myClient;
	char	buff[1024];

	myClient = findClientByFd(fd);
	//-> buffer for the received data
	std::memset(buff, 0, sizeof(buff));                  //-> clear the buffer
	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1, 0); //-> receive the data
	if (bytes <= 0)
	{
		std::cout << RED << "Client <" << fd << "> Disconnected" << END << std::endl;
		clearClient(fd);
		close(fd);
	}
	else
	{
		buff[bytes] = '\0';
		std::cout << YELLOW << "Client <" << fd << "> Data: " << END << buff;
		std::string myStr = buff;
		// addToBuff(myStr, myClient);
		myClient.addBuff(myStr);
		if (myStr.find("/r/n"))
		{
			execute(myClient);
			myClient.clearBuffer();
		}
		// std::cout << myClient.getBuffer() << std::endl;
	}
}

void Server::setPort(int port)
{
	this->_port = port;
}

void Server::setPassword(std::string pswd)
{
	this->_password = pswd;
}

void Server::addToBuff(std::string data, Client &myClient)
{
	myClient.addBuff(data);
}

Client &Server::findClientByFd(int fd)
{
	std::vector<Client>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->getFd() == fd)
			return (*it);
	}
	// throw(std::exception);
	return (*(_clients.end()));
}
static int	parse(std::string cmd)
{
	size_t	i;

	if (cmd.empty())
		return (-1);
	std::string name;
	std::size_t name_end = cmd.find_first_of(' ');
	if (name_end != std::string::npos)
		name = cmd.substr(0, name_end);
	else
		name = cmd;
	std::string commands[9] = {"PASS", "NICK", "USER", "JOIN", "PRIVMSG",
		"MODE", "KICK", "INVITE", "TOPIC"};
	i = 0;
	while (i < 9 && name != commands[i])
		++i;
	return (i);
}

void Server::execute(Client cli)
{
	int	cmdIdx;

	cmdIdx = parse(cli.getBuffer());
	if (cmdIdx < 0 || cmdIdx > 8)
		return ; //! commande inconnue ou vide, faut voir quoi renvoyer
	// void (*commands[9])(Client) = {cmdPass, cmdNick, cmdUser, cmdJoin,
	// cmdPrivMsg, cmdMode, cmdKick, cmdInvite, cmdTopic};
	// commands[cmdIdx](cli);
	return ;
}
