
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
	std::string commands[10] = {"PASS", "NICK", "USER", "JOIN", "PRIVMSG",
								"MODE", "KICK", "INVITE", "TOPIC", "1"};
	i = 0;
	while (i < 10 && name != commands[i])
		++i;
	return (i);
}


void Server::execute(Client &cli)
{
	int cmdIdx;

	void (Server::*commands[])(Client &) = {&Server::cmdPass, &Server::cmdNick,
											&Server::cmdUser, &Server::cmdJoin, &Server::cmdPrivMsg, &Server::cmdMode, &Server::cmdKick, &Server::cmdInvite, &Server::cmdTopic, &Server::cmdTest};

	cmdIdx = parse(cli.getBuffer());
	if (cmdIdx < 0 || cmdIdx > 9)
	{
		std::cerr << "Error: unknown command: " << cli.getBuffer() << std::endl;
		return; //! commande inconnue ou vide, faut voir quoi renvoyer
	}
	try 
	{
		(this->*commands[cmdIdx])(cli);
	}
	catch (std::exception &e) 
	{
		std::cout << e.what() << std::endl;
	}
	// return;
	// if (!this->_password.empty() && cli.getAuthStatus() == 0 && cmdIdx != 0)
	// {
	// 	std::cerr << "Error tried to log without password" << std::endl;
	// 	return; //! frerot faut mettre un mdp
	// }
	// else if (cli.getAuthStatus() < 2 && cmdIdx > 2)
	// {
	// 	std::cerr << "Error tried to cmd without log" << std::endl;
	// 	return; //! frerot log toi
	// }
	// else
	// 	// commands[cmdIdx](cli);
	// 	return;
}