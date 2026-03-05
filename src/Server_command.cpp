#include "Server.hpp"

void Server::cmdCap(Client &cli, std::string cmd)
{
	std::vector<std::string> tokens = split(cmd, ' ');
	if (tokens.size() != 2 && tokens[1] == "LS")
	{
		std::string msg = "CAP * LS";
		cli.sendMessageOnClientFd(msg);
	}
	return; // We don't support CAP so we ignore
}



void Server::cmdQuit(Client &cli, std::string cmd)
{
	std::vector<std::string> tokens = split(cmd, ' ');
	std::string reason = ":Client Quit";
	if (tokens.size() > 1 && tokens[1][0] == ':')
		reason = tokens[1];
	std::string nick = cli.getNickName();
	std::string msg = ":" + cli.getPrefix() + " QUIT " + reason;
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (this->_channels[i]->getClient(nick) != NULL)
			this->_channels[i]->sendChannelMessage(cli, msg);
	}
	cli.setToClean(1);
}

void Server::cmdPass(Client &myClient, std::string cmd)
{
	std::string passWord;
	std::size_t nameStart = cmd.find_first_of(' ');
	if (nameStart == cmd.size() || nameStart == std::string::npos)
	{
		std::string msg = ":ft_irc 461 " + myClient.getNickName() + " PASS :Not enough parameters";
		myClient.sendMessageOnClientFd(msg);
		return;
	}
	passWord = cmd.substr(nameStart + 1);
	//?be more specifiques on the spaces rules for password
	if (passWord == _password)
	{
		if (myClient.getAuthStatus() > 0)
		{
			std::string msg = ":ft_irc 462 " + myClient.getNickName() + " :You may not reregister";
			myClient.sendMessageOnClientFd(msg);
			return;
		}
		myClient.setGrade(1);
	}
	else
	{
		std::string msg = ":ft_irc 464 " + myClient.getNickName() + " :Password incorrect";
		myClient.sendMessageOnClientFd(msg);
	}
}

void Server::broadcastNick(Client &cli, std::string &nick)
{

	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (_channels[i]->getClient(cli.getNickName()) != NULL)
		{
			std::string msg = ":" + cli.getPrefix() + " NICK :" + nick + "\n";
			_channels[i]->sendChannelMessage(cli, msg);
		}
	}
}

void Server::cmdNick(Client &myClient, std::string cmd)
{
	std::string nickname;
	std::size_t nameStart = cmd.find_first_of(' ');
	if (nameStart == cmd.size() || nameStart == std::string::npos)
	{
		std::string msg = ":ft_irc 431 " + myClient.getNickName() + " :No nickname given";
		myClient.sendMessageOnClientFd(msg);
		return;
	}
	nickname = cmd.substr(nameStart + 1);
	if (isValidString(nickname))
	{
		if (findFdByNickName(nickname) == -1)
		{
			broadcastNick(myClient, nickname);
			myClient.setNickName(nickname);
			if (!(myClient.getUserName()).empty())
				myClient.setGrade(2);
		}
		else
		{
			std::string msg = ":ft_irc 433 " + myClient.getNickName() + " " + nickname + " :Nickname is already in use";
			myClient.sendMessageOnClientFd(msg);
		}
	}
	else
	{
		std::string msg = ":ft_irc 432 " + myClient.getNickName() + " " + nickname + " :Erroneus nickname";
		myClient.sendMessageOnClientFd(msg);
	}
}

void Server::cmdUser(Client &myClient, std::string cmd)
{
	std::vector<std::string> tokens = split(cmd, ' ');
	if (tokens.size() < 5)
	{
		std::string msg = ":ft_irc 461 " + myClient.getNickName() + " USER :Not enough parameters";
		myClient.sendMessageOnClientFd(msg);
		return;
	}
	if (!(myClient.getUserName()).empty())
	{
		std::string msg = ":ft_irc 462 " + myClient.getNickName() + " :You may not reregister";
		myClient.sendMessageOnClientFd(msg);
		return;
	}
	myClient.setUserName(tokens[1]);
	std::string realname = tokens[4].substr(1);
	for (size_t i = 5; i < tokens.size(); i++)
		realname += " " + tokens[i];
	myClient.setRealName(realname);
	if (!(myClient.getNickName()).empty() && myClient.getNickName() != "*")
		myClient.setGrade(2);
}

void Server::clearEmptyChannel(void)
{
	std::vector<Channel *>::iterator it = _channels.begin();

	while (it != _channels.end())
	{
		if ((*it)->getClientsSize() == 0)
		{
			delete (*it);
			it = _channels.erase(it);
		}
		else
			it++;
	}
}
