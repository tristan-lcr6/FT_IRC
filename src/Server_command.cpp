#include "Server.hpp"

void Server::cmdCap(Client &cli, std::string cmd)
{
	std::vector<std::string> tokens = split(cmd, ' ');
	if (tokens.size() != 2 && tokens[1] == "LS")
	{
		std::string msg = "CAP * LS";
		cli.sendMessageOnClientFd(msg);
	}
	return ; // We don't support CAP so we ignore
}

void Server::cmdPass(Client &myClient, std::string cmd)
{
	std::cout << myClient << std::endl;
	std::string passWord;
	std::size_t nameStart = cmd.find_first_of(' ');
	if (nameStart == cmd.size() || nameStart == std::string::npos)
	{
		std::string msg = ":ft_irc 461 " + myClient.getNickName() + " PASS :Not enough parameters";
		myClient.sendMessageOnClientFd(msg);
		return ;
	}
	passWord = cmd.substr(nameStart + 1);
	//?be more specifiques on the spaces rules for password
	std::cout << "passWord is '" << passWord << "'" << std::endl;
	if (passWord == _password)
	{
		if (myClient.getAuthStatus() > 0)
		{
			std::string msg = ":ft_irc 462 " + myClient.getNickName() + " :You may not reregister";
			myClient.sendMessageOnClientFd(msg);
			return ;
		}
		myClient.setGrade(1);
		std::cout << myClient << std::endl;
	}
	else
	{
		std::string msg = ":ft_irc 464 " + myClient.getNickName() + " :Password incorrect";
		myClient.sendMessageOnClientFd(msg);
	}
}


void Server::cmdTest(Client &myClient, std::string cmd)
{
	std::cout << myClient << std::endl;
	std::string passWord;
	std::size_t nameStart = cmd.find_first_of(' ');
	if (nameStart == cmd.size())
		return;
	passWord = cmd.substr(nameStart + 1);
	//?be more specifiques on the spaces rules for password
	myClient.setGrade(2);
	myClient.setNickName(passWord);
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
	std::cout << myClient << std::endl;
	std::string nickname;
	std::size_t nameStart = cmd.find_first_of(' ');
	if (nameStart == cmd.size() || nameStart == std::string::npos)
	{
		std::string msg = ":ft_irc 431 " + myClient.getNickName() + " :No nickname given";
		myClient.sendMessageOnClientFd(msg);
		return ;
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
			std::cout << myClient << std::endl;
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
	myClient.setRealName(tokens[4].substr(1));
	if (!(myClient.getNickName()).empty() && myClient.getNickName() != "*")
		myClient.setGrade(2);
	std::cout << myClient << std::endl;

}
