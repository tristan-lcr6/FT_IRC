#include "Server.hpp"

// void Server::addToBuff(std::string data, Client &myClient)
// {
// 	myClient.addBuff(data);
// }

static int parse(std::string cmd)
{
	size_t i;

	if (cmd.empty())
		return (-1);
	std::string name;
	std::size_t name_end = cmd.find_first_of(' ');
	if (name_end != std::string::npos)
		name = cmd.substr(0, name_end);
	else
		name = cmd;
	std::string commands[11] = {"CAP", "PASS", "NICK", "USER", "JOIN", "PRIVMSG",
								"MODE", "KICK", "INVITE", "TOPIC", "1"};
	i = 0;
	while (i < 11 && name != commands[i])
		++i;
	return (i);
}

void Server::execute(Client &cli, std::string cmd)
{
	int cmdIdx;
	void (Server::*commands[])(Client &, std::string) = {
		&Server::cmdCap,
		&Server::cmdPass,
		&Server::cmdNick,
		&Server::cmdUser,
		&Server::cmdJoin,
		&Server::cmdPrivMsg,
		&Server::cmdMode,
		&Server::cmdKick,
		&Server::cmdInvite,
		&Server::cmdTopic,
		&Server::cmdTest
	};
	cmdIdx = parse(cmd);
	if (cmdIdx < 0 || cmdIdx > 10)
	{
		std::cerr << "Error: unknown command: " << cmd << std::endl;
		return; //! commande inconnue ou vide, faut voir quoi renvoyer
	}
	try
	{
		int pre_auth = cli.getAuthStatus();

		// If password is required and not provided yet,
		// only allow CAP and PASS
		if (!this->_password.empty() && cli.getAuthStatus() == 0 && cmdIdx != 0 // CAP
			&& cmdIdx != 1) // PASS
		{
			std::cerr << "Error: authentication required" << std::endl;
			return;
		}

		// If not fully registered, only allow PASS/NICK/USER/CAP
		if (cli.getAuthStatus() < 2 && cmdIdx > 3)
		{
			std::cerr << "Error: client not registered yet" << std::endl;
			return;
		}

		// Execute command
		(this->*commands[cmdIdx])(cli, cmd);

		int post_auth = cli.getAuthStatus();

		// Send welcome numeric when registration completes
		if (pre_auth < 2 && post_auth == 2)
		{
			std::string msg =
				":ft_irc 001 " + cli.getNickName() +
				" :Welcome to the Internet Relay Network " +
				cli.getPrefix() + "\r\n";

			cli.sendMessageOnClientFd(msg);
		}
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
