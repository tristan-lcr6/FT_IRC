
#include "Server.hpp"

void Server::serverInit()
{
	serSocket();
	std::cout << GREEN << "Server <" << this->_serSocketFd << "> Connected" << END << std::endl;
	std::cout << "Waiting to accept a connection...\n";
	while (Server::_signal == false)
	{
		if ((poll(&this->_fds[0], this->_fds.size(), -1) == -1) && Server::_signal == false)
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


void Server::serSocket()
{
	struct sockaddr_in add;
	struct pollfd NewPoll;
	int en;

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



void Server::acceptNewClient()
{
	struct sockaddr_in cliadd;
	struct pollfd NewPoll;
	socklen_t len;
	Client cli;
	int incofd;

	len = sizeof(cliadd);
	incofd = accept(this->_serSocketFd, (sockaddr *)&(cliadd), &len);
	if (incofd == -1)
	{
		std::cout << "accept() failed" << std::endl;
		return;
	}
	if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cout << "fcntl() failed" << std::endl;
		return;
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
    Client *client = findClientByFd(fd);
    if (!client)
        return;

    char buff[1024];
    ssize_t bytes = recv(fd, buff, sizeof(buff), 0); // receive the data

    if (bytes <= 0)
    {
        std::cout << RED << "Client <" << fd << "> Disconnected" << END << std::endl;
        clearClient(fd);
        close(fd);
        return;
    }

    // Append received data to client's buffer
    client->addBuff(std::string(buff, bytes));

    std::string &buffer = client->getBuffer();
    size_t pos;

    // Process ALL complete commands
    while ((pos = buffer.find("\r\n")) != std::string::npos)
    {
        std::string line = buffer.substr(0, pos);
        buffer.erase(0, pos + 2);

        std::cout << YELLOW << "Client <" << fd << "> Parsed: " << line << END << std::endl;

        execute(*client, line);
    }
}

// void Server::receiveNewData(int fd)
// {
// 	Client *myClient = NULL;
// 	char buff[1024];
// 	size_t end_start;

// 	myClient = &findClientByFd(fd);
// 	std::cout << YELLOW << "Client <" << fd << "> Data: " << END << buff;

// 	//-> buffer for the received data
// 	std::memset(buff, 0, sizeof(buff));					 //-> clear the buffer
// 	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1, 0); //-> receive the data
// 	if (bytes <= 0)
// 	{
// 		std::cout << RED << "Client <" << fd << "> Disconnected" << END << std::endl;
// 		clearClient(fd);
// 		close(fd);
// 	}
// 	else
// 	{
// 		buff[bytes] = '\0';
// 		std::cout << YELLOW << "Client <" << fd << "> Data: " << END << buff;
// 		std::string myStr = buff;
// 		(*myClient).addBuff(myStr);
// 		if (myStr.find("\r\n") != std::string::npos)
// 		{
// 			end_start = (*myClient).getBuffer().find_first_of("\r\n");
// 			(*myClient).setBuffer((*myClient).getBuffer().erase(end_start,
// 																(*myClient).getBuffer().size()));
// 			if (!myClient)
// 				std::cout << RED << "Client lost" << END << std::endl;
// 			else
// 				execute(*myClient);
// 			(*myClient).clearBuffer();
// 		}
// 		// std::cout << myClient.getBuffer() << std::endl;
// 	}
// }

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


void Server::clearClient(int fd)
{
	for (size_t i = 0; i < this->_fds.size(); i++)
	{
		if (this->_fds[i].fd == fd)
		{
			this->_fds.erase(this->_fds.begin() + i);
			break;
		}
	}
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i].getFd() == fd)
		{
			this->_clients.erase(this->_clients.begin() + i);
			break;
		}
	}
}


void Server::signalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl
			  << "Signal Received!" << std::endl;
	Server::_signal = true;
}