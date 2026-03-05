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
	std::string commands[15] = {"CAP", "PASS", "NICK", "USER", "JOIN", "PRIVMSG",
								"MODE", "KICK", "INVITE", "TOPIC", "NAMES", "WHO", "QUIT", "PING", "BOT"};
	i = 0;
	while (i < 15 && name != commands[i])
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
		&Server::cmdNames,
		&Server::cmdWho,
		&Server::cmdQuit,
		&Server::cmdPing,
		&Server::cmdBot
	};
	cmdIdx = parse(cmd);
	if (cmdIdx < 0 || cmdIdx > 14)
	{
		std::string msg = ":ft_irc 421 " + cli.getNickName() + " " + cmd + " :Unknown command";
		cli.sendMessageOnClientFd(msg);
		return; //! commande inconnue
	}
	try
	{
		int pre_auth = cli.getAuthStatus();

		// If password is required and not provided yet,
		// only allow CAP and PASS
		if (!this->_password.empty() && cli.getAuthStatus() == 0 && cmdIdx != 0 // CAP
			&& cmdIdx != 1)														// PASS
		{
			std::string msg = ":ft_irc 464 " + cli.getNickName() + " :Password incorrect";
			cli.sendMessageOnClientFd(msg);
			return;
		}

		// If not fully registered, only allow PASS/NICK/USER/CAP
		if (cli.getAuthStatus() < 2 && cmdIdx > 3)
		{
			std::string msg = ":ft_irc 451 " + cli.getNickName() + " :You have not registered";
			cli.sendMessageOnClientFd(msg);
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
				cli.getPrefix();

			cli.sendMessageOnClientFd(msg);
		}
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void Server::cmdPing(Client &cli, std::string cmd)
{
	std::vector<std::string> tokens = split(cmd, " ");
	if (tokens[0] == "PING")
	{
		if (tokens.size() < 2)
		{
			// Erreur : pas assez de paramètres (461)
			return;
		}
		std::string param = tokens[1];
		std::string response = ":ft_irc.fr PONG :" + param + "\r\n";
		cli.sendMessageOnClientFd(response);
	}
}
