/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jferrand <jferrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 11:41:46 by jferrand          #+#    #+#             */
/*   Updated: 2026/02/27 12:06:17 by jferrand         ###   ########.fr       */
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
	Client	*myClient = NULL;
	char	buff[1024];
	size_t	end_start;

	myClient = &findClientByFd(fd);
	std::cout << YELLOW << "Client <" << fd << "> Data: " << END << buff;

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
		(*myClient).addBuff(myStr);
		if (myStr.find("\r\n") != std::string::npos)
		{
			end_start = (*myClient).getBuffer().find_first_of("\r\n");
			(*myClient).setBuffer((*myClient).getBuffer().erase(end_start,
					(*myClient).getBuffer().size()));
			if(!myClient)
				std::cout << RED << "Client lost" << END << std::endl;				
			else 
				execute(*myClient);
			(*myClient).clearBuffer();
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
	return (*(_clients.end()));//!change
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

void Server::execute(Client &cli)
{
	int	cmdIdx;

	void (Server::*commands[])(Client &) = {&Server::cmdPass, &Server::cmdNick,
		&Server::cmdUser, &Server::cmdJoin, &Server::cmdPrivMsg, &Server::cmdMode, &Server::cmdKick, &Server::cmdInvite, &Server::cmdTopic};
	
	cmdIdx = parse(cli.getBuffer());
	if (cmdIdx < 0 || cmdIdx > 8)
	{
		std::cerr << "Error: unknown command: " << cli.getBuffer() << std::endl;
		return ; //! commande inconnue ou vide, faut voir quoi renvoyer
	}
	(this->*commands[cmdIdx])(cli);
	return ;
	if (!this->_password.empty() && cli.getAuthStatus() == 0 && cmdIdx != 0)
	{
		std::cerr << "Error tried to log without password" << std::endl;
		return ; //! frerot faut mettre un mdp
	}
	else if (cli.getAuthStatus() < 2 && cmdIdx > 2)
	{
		std::cerr << "Error tried to cmd without log" << std::endl;
		return ; //! frerot log toi
	}
	else
		// commands[cmdIdx](cli);
		return ;
	}


void Server::cmdPass(Client &myClient)
{
	std::cout << myClient << std::endl;
	std::string passWord;
	std::size_t nameStart = (myClient.getBuffer()).find_first_of(' ');
	if (nameStart == myClient.getBuffer().size())
		return ;
	passWord = (myClient.getBuffer()).substr(nameStart + 1);
	//?be more specifiques on the spaces rules for password
	std::cout << "passWord is'" << passWord << "'" << std::endl;
	if (passWord == _password)
	{
		myClient.setGrade(1);
		std::cout << myClient << std::endl;
	}
	else
		std::cout << "Error :wrong Password." << std::endl;
}

void Server::cmdNick(Client &myClient)
{
				std::cout << myClient << std::endl;
	std::string nickname;
	std::size_t nameStart = (myClient.getBuffer()).find_first_of(' ');
	if (nameStart == myClient.getBuffer().size()
		|| nameStart == std::string::npos)
		return ;
	nickname = (myClient.getBuffer()).substr(nameStart + 1);
	if (isValidString(nickname))
	{
		if (!findNickName(nickname))
		{
			myClient.setNickName(nickname);
				std::cout << myClient << std::endl;
		}
		std::cout << "Error : Nickame already used." << std::endl;
	}
	else
		std::cout << "Error :Not a valid Nickame." << std::endl;
}
void Server::cmdUser(Client &myClient)
{
	std::string realname;
	std::string cpy = myClient.getBuffer();
	if (cpy.find(":") != std::string::npos)
	{
		realname = cpy.substr(cpy.find(":"));
		cpy.erase(cpy.find(":") - 1);
	}
	std::vector<std::string> tokens;
	tokens = split(cpy, " ");
	std::cout << tokens << std::endl;
	std::cout << "realname is :" << realname << "." << std::endl;
	if (tokens.size() != 4)
		std::cout << "Error :Not a valid User entry." << std::endl;
	myClient.setGrade(2);
}

bool	isValidString(const std::string &str)
{
	char	c;

	if (str.empty())
		return (false);
	for (size_t i = 0; i < str.length(); ++i)
	{
		c = str[i];
		if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0'
					&& c <= '9') || c == '_'))
		{
			return (false);
		}
	}
	return (true);
}

// JOIN #a,#b,#c passA,passB,passC
// If the channel exists and can join, joins it
// If it doesn't exist create it
void Server::cmdJoin(Client &cli)
{
	std::vector<std::string> tokens = split(cli.getBuffer(), ' ');
	std::vector<std::string> channels = split(tokens[1], ',');
	std::vector<std::string> passwords;
	if (tokens.size() > 2)
		passwords = split(tokens[2], ',');
	for (std::size_t i = 0; i < channels.size(); i++)
	{
		bool found = false;
		std::string name = channels[i];
		for (std::size_t j = 0; j < this->_channels.size(); j++)
		{
			if (name == this->_channels[j].getName())
			{
				if (i < passwords.size() && !passwords[i].empty())
					this->_channels[j].join(cli, passwords[i]);
				else
					this->_channels[j].join(cli);
				found = true;
				break;
			}
		}
		if (!found)
		{
			this->_channels.push_back(Channel(cli, name));
			std::cout << "Channel " << name << " created" << std::endl;
		}
	}
}

// MODE <channel> <modes> [params]
// MODE #test +i
// MODE #test +k secret
// MODE #test +o jesus
// MODE #test +l 10
// MODE #test -k
// Verifies that cli is an operator then executes the asked action
void Server::cmdMode(Client &cli)
{
	std::vector<std::string> tokens = split(cli.getBuffer(), ' ');
	if (tokens.size() < 3)
	{
		std::cerr << "Error: not enough arguments for MODE" << std::endl;
		return ; //!
	}
	std::string channel_name = tokens[1];
	std::cout << "Moding " << channel_name << " channel" << std::endl;
	Channel *channel = NULL;
	if (!this->findChannel(channel, channel_name))
	{
		std::cerr << "Error: tried to mode an unknown channel: " << channel_name << std::endl;
		return ; //! Channel doesn't exist
	}
	std::string modestring = tokens[2];
	std::vector<std::string> params;
	if (tokens.size() > 3)
		params.assign(tokens.begin() + 3, tokens.end());;
	std::size_t paramIdx = 0;
	bool add = true;
	for (size_t i = 0; i < modestring.size(); ++i)
	{
		char c = modestring[i];
		if (c == '+')
		{
			add = true;
			continue;
		}
		if (c == '-')
		{
			add = false;
			continue;
		}
		if (Channel::modeWithParam(c, add))
		{
			if (paramIdx >= params.size())
            {
				std::cerr << "Error: the mode " << c << " needs params" << std::endl;
				return ; //! error not enough params
			}
			channel->applyMode(c, add, params[paramIdx++]);
		}
		else
			channel->applyMode(c, add);
	}
}

bool Server::findChannel(Channel *&channel, const std::string &channel_name)
{
	for (std::size_t i = 0; i < this->_channels.size(); i++)
	{
		if (channel_name == this->_channels[i].getName())
		{
			channel = &this->_channels[i];
			return true;
		}
	}
	return false;
}

// KICK <channel> <nick> [:reason]
// KICK #test john :spamming
void Server::cmdKick(Client &cli)
{
	std::vector<std::string> tokens = split(cli.getBuffer(), ' ');
	if (tokens.size() < 3)
	{
		std::cerr << "Error: not enough arguments for KICK" << std::endl;
		return ; //!
	}
	std::string channel_name = tokens[1];
	std::string nick = tokens[2];
	std::string reason;
	if (tokens.size() > 3)
	{
		if (tokens[3][0] != ':')
		{
			std::cerr << "Error: bad arguments for KICK, missing : before reason" << std::endl;
			return ; //!
		}
		reason = tokens[3].substr(1, tokens[3].size());
		for (size_t i = 4; i < tokens.size(); i++)
			reason += tokens[i];
	}
	Channel *channel = NULL;
	if (!this->findChannel(channel, channel_name))
	{
		std::cerr << "Error: tried to kick in an unknown channel: " << channel_name << std::endl;
		return ; //! Channel doesn't exist
	}
	channel->kick(*channel->getClient(nick));
	std::cout << nick << " kicked succesfully" << std::endl;
}

void Server::cmdPrivMsg(Client &cli)
{
	(void)cli;
}

// INVITE <nick> <channel>
// si le channel est invite-only seulement un operateur peut inviter
void Server::cmdInvite(Client &cli)
{
	std::vector<std::string> tokens = split(cli.getBuffer(), ' ');
	if (tokens.size() != 3)
	{
		std::cerr << "Error: not enough arguments for INVITE" << std::endl;
		return ; //!
	}
	std::string nick = tokens[1];
	std::string channel_name = tokens[2];
	Channel *channel = NULL;
	if (!this->findChannel(channel, channel_name))
	{
		std::cerr << "Error: tried to kick in an unknown channel: " << channel_name << std::endl;
		return ; //! Channel doesn't exist
	}
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i].getNickname() == nick)
		{
			channel->invite(this->_clients[i]);
			std::cout << nick << " invited to " << channel_name << std::endl;
			break;
		}
	}
}

// TOPIC <channel> [:topic]
// TOPIC #test -> affiche le topic
// TOPIC #test :New topic -> change le topic
// si channel +t alors le changement est op-only
void Server::cmdTopic(Client &cli)
{
	std::vector<std::string> tokens = split(cli.getBuffer(), ' ');
	if (tokens.size() < 2)
	{
		std::cerr << "Error: not enough arguments for TOPIC" << std::endl;
		return ; //!
	}
	std::string channel_name = tokens[1];
	std::string topic;
	Channel *channel = NULL;
	if (!this->findChannel(channel, channel_name))
	{
		std::cerr << "Error: tried to kick in an unknown channel: " << channel_name << std::endl;
		return ; //! Channel doesn't exist
	}
	if (tokens.size() == 2)
	{
		topic = channel->getTopic();
		std::cout << "Topic is: " << topic << std::endl; //! a envoyer au client
		return ;
	}
	if (tokens[2][0] != ':')
	{
		std::cerr << "Error: bad arguments for TOPIC, missing : before topic" << std::endl;
		return ; //!
	}
	topic = tokens[2].substr(1, tokens[2].size());
	for (size_t i = 3; i < tokens.size(); i++)
		topic += tokens[i];
	channel->setTopic(topic);
	std::cout << "Topic set to: " << topic << std::endl;
}

int Server::findNickName(std::string nickName)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getNickName() == nickName)
			return (1);
	}
	return (0);
}

std::ostream& operator<<(std::ostream& dataStream, const std::vector<std::string>& vector) 
{
    dataStream << "[";
    for (size_t i = 0; i < vector.size(); ++i) {
        dataStream << vector[i];
        if (i != vector.size() - 1) {
            dataStream << ", ";
        }
    }
    dataStream << "]";
    return dataStream;
}
