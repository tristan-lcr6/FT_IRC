#include "Server.hpp"

void Server::JoinMessage(std::string channelName, Client &cli)
{

	std::string nick = cli.getNickName();
	std::string user = cli.getUserName();
	std::string host = "localhost"; // Ou l'IP réelle
	std::string serverName = "irc.42.fr";
	std::string const prefix = cli.getPrefix();
	cli.sendMessageOnClientFd(prefix + " JOIN " + channelName);
	cli.sendMessageOnClientFd(prefix + " 332 " + channelName + " :Bienvenue sur " + channelName);
	cli.sendMessageOnClientFd(":" + serverName + " 333 " + nick + " " + channelName + " " + nick + " 1672531200");

	// 4. Envoyer la liste des noms (RPL_NAMsendMessageOnClientFd 353)
	// Ici tu dois boucler sur tous les membres du channel pour construire la liste
	// std::string list = "@" + nick + " member2 member3";
	// cli.sendMessageOnClientFd(":" + serverName + " 353 " + nick + " = " + channelName + " :" + list);
	// cli.sendMessageOnClientFd(":" + serverName + " 366 " + nick + " " + channelName + " :End of /NAMES list");
}

// JOIN #a,#b,#c passA,passB,passC
// If the channel exists and can join, joins it
// If it doesn't exist create it
void Server::cmdJoin(Client &cli, std::string cmd)
{
	std::vector<std::string> tokens = split(cmd, ' ');
	if (tokens.size() < 2)
	{
		std::string msg = ":ft_irc 461 " + cli.getNickName() + " JOIN :Not enough parameters";
		cli.sendMessageOnClientFd(msg);
		return ;
	}
	std::vector<std::string> channels = split(tokens[1], ',');
	std::vector<std::string> passwords;
	if (tokens.size() > 2)
		passwords = split(tokens[2], ',');
	for (std::size_t i = 0; i < channels.size(); i++)
	{

		std::string name = channels[i];
		if (name[0] != '#')
		{
			std::string msg = ":ft_irc 403 " + cli.getNickName() + " " + name + " :No such channel";
			cli.sendMessageOnClientFd(msg);
		}
		bool found = false;
		for (std::size_t j = 0; j < this->_channels.size(); j++)
		{
			if (name == this->_channels[j]->getName())
			{
				std::cout << "Adresse prejoin: " << &cli << std::endl;
				if (i < passwords.size())
					this->_channels[j]->join(cli, passwords[i]);
				else
					this->_channels[j]->join(cli);
				// std::cout << "join existing channel" << *(this->_channels[j]) << std::endl;
				JoinMessage(name, cli);
				found = true;
				break;
			}
		}
		if (!found)
		{
			std::cout << "Adresse prejoin: " << &cli << std::endl;
			Channel *newChan = new Channel(cli, name);
			this->_channels.push_back(newChan);
			// std::cout << "Channel " << name << " created and client added." << std::endl;
			std::cout << *newChan << std::endl;
			JoinMessage(name, cli);
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
void Server::cmdMode(Client &cli, std::string cmd)
{
	std::vector<std::string> tokens = split(cmd, ' ');
	if (tokens.size() < 3)
	{
		std::string msg = ":ft_irc 461 " + cli.getNickName() + " MODE :Not enough parameters";
		cli.sendMessageOnClientFd(msg);
		return; //!
	}
	std::string channel_name = tokens[1];
	if (channel_name[0] != '#')
		return ; //* ignore user modes
	Channel *channel = NULL;
	if (!this->isAlreadyChannel(&channel, channel_name))
	{
		std::string msg = ":ft_irc 403 " + cli.getNickName() + " " + channel_name + " :No such channel";
		cli.sendMessageOnClientFd(msg);
		return; //! Channel doesn't exist
	}
	if (channel->getClient(cli.getNickName()) == NULL || !channel->isOperator(cli.getNickName()))
	{
		std::string msg = ":ft_irc 482 " + cli.getNickName() + " " + channel_name + " :You're not channel operator";
		cli.sendMessageOnClientFd(msg);
		return; //! Client not in channel or not operator
	}
	std::string modestring = tokens[2];
	std::vector<std::string> params;
	if (tokens.size() > 3)
		params.assign(tokens.begin() + 3, tokens.end());
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
				std::string msg = ":ft_irc 461 " + cli.getNickName() + " MODE :Not enough parameters";
				cli.sendMessageOnClientFd(msg);
				continue; //! error not enough params
			}
			channel->applyMode(c, add, params[paramIdx++]);
		}
		else
			channel->applyMode(c, add);
	}
}

// KICK <channel> <nick> [:reason]
// KICK #test john :spamming
void Server::cmdKick(Client &cli, std::string cmd)
{
	std::vector<std::string> tokens = split(cmd, ' ');
	if (tokens.size() < 3)
	{
		std::string msg = ":ft_irc 461 " + cli.getNickName() + " KICK :Not enough parameters";
		cli.sendMessageOnClientFd(msg);
		return; //!
	}
	std::string channel_name = tokens[1];
	std::string nick = tokens[2];
	std::string reason;
	if (tokens.size() > 3)
	{
		if (tokens[3][0] != ':')
		{
			std::string msg = ":ft_irc 461 " + cli.getNickName() + " KICK :Not enough parameters";
			cli.sendMessageOnClientFd(msg);
			return; //!
		}
		reason = tokens[3].substr(1);
		for (size_t i = 4; i < tokens.size(); i++)
			reason += tokens[i];
	}
	Channel *channel = NULL;
	if (!this->isAlreadyChannel(&channel, channel_name))
	{
		std::string msg = ":ft_irc 403 " + cli.getNickName() + " " + channel_name + " :No such channel";
		cli.sendMessageOnClientFd(msg);
		return; //! Channel doesn't exist
	}
	// std::cout << channel << " channel contain :  -------------------\n" <<  *channel << std::endl;
	if (channel->getClient(cli.getNickName()) == NULL || !channel->isOperator(cli.getNickName()))
	{
		std::string msg = ":ft_irc 482 " + cli.getNickName() + " " + channel_name + " :You're not channel operator";
		cli.sendMessageOnClientFd(msg);
		return; //! Client not in channel or not operator
	}
	if (channel->getClient(nick) == NULL)
	{
		std::string msg = ":ft_irc 441 " + cli.getNickName() + " " + nick + " " + channel_name + " :They aren't on that channel";
		cli.sendMessageOnClientFd(msg);
		return; //! nickname not in channel
	}
	channel->kick(*channel->getClient(nick));
	std::cout << nick << " kicked succesfully" << std::endl;
}

// Channel &Server::getChannel(std::string Channelname)
// {
// 	for (size_t i = 0; i < _channels.size();i++)
// 	{
// 		if(_channels[i].getName() == Channelname)
// 		{
// 			return (_channels[i]);
// 		}
// 	}
// 	std::cout << "Error :could not find channel named "<< Channelname << std::endl;
// 	return (_channels[0]);//! throw exception instead
// }

// INVITE <nick> <channel>
// si le channel est invite-only seulement un operateur peut inviter
// envoyer :ft_irc 341 <inviter> <target> <channel> a l'inviteur
// envoyer :nick INVITE target :#channel a l'invite
void Server::cmdInvite(Client &cli, std::string cmd)
{
	std::vector<std::string> tokens = split(cmd, ' ');
	if (tokens.size() != 3)
	{
		std::string msg = ":ft_irc 461 " + cli.getNickName() + " INVITE :Not enough parameters";
		cli.sendMessageOnClientFd(msg);
		return; //!
	}
	std::string nick = tokens[1];
	std::string channel_name = tokens[2];
	Channel *channel = NULL;
	if (!this->isAlreadyChannel(&channel, channel_name))
	{
		std::string msg = ":ft_irc 403 " + cli.getNickName() + " " + channel_name + " :No such channel";
		cli.sendMessageOnClientFd(msg);
		return; //! Channel doesn't exist
	}
	if (channel->getClient(cli.getNickName()) == NULL || !channel->isOperator(cli.getNickName()))
	{
		std::string msg = ":ft_irc 482 " + cli.getNickName() + " " + channel_name + " :You're not channel operator";
		cli.sendMessageOnClientFd(msg);
		return; //! Client not in channel or not operator
	}
	if (channel->getClient(nick) != NULL)
	{
		std::string msg = ":ft_irc 443 " + cli.getNickName() + " " + nick + " " + channel_name + " :is already on channel";
		cli.sendMessageOnClientFd(msg);
		return; //! target already in channel
	}
	bool found = false;
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i]->getNickName() == nick)
		{
			channel->invite(*this->_clients[i]);
			std::string inviterMsg = ":ft_irc 341 " + cli.getNickName() + " " + nick + " " + channel_name;
			std::string invitedMsg = ":" + cli.getNickName() + " INVITE " + nick + " :" + channel_name;
			cli.sendMessageOnClientFd(inviterMsg);
			this->_clients[i]->sendMessageOnClientFd(invitedMsg);
			found = true;
			break;
		}
	}
	if (!found)
	{
		std::string msg = ":ft_irc 401 " + cli.getNickName() + " " + nick + " :No such nick";
		cli.sendMessageOnClientFd(msg);
	}
}

// TOPIC <channel> [:topic]
// TOPIC #test -> affiche le topic
// TOPIC #test :New topic -> change le topic
// si channel +t alors le changement est op-only
// envoyer :<nick>!<user>@<host> TOPIC <channel> :<new topic> sur le channel
void Server::cmdTopic(Client &cli, std::string cmd)
{
	std::vector<std::string> tokens = split(cmd, ' ');
	if (tokens.size() < 2)
	{
		std::string msg = ":ft_irc 461 " + cli.getNickName() + " TOPIC :Not enough parameters";
		cli.sendMessageOnClientFd(msg);
		return; //!
	}
	std::string channel_name = tokens[1];
	std::string topic;
	Channel *channel = NULL;
	if (!this->isAlreadyChannel(&channel, channel_name))
	{
		std::string msg = ":ft_irc 403 " + cli.getNickName() + " " + channel_name + " :No such channel";
		cli.sendMessageOnClientFd(msg);
		return; //! Channel doesn't exist
	}
	if (channel->getClient(cli.getNickName()) == NULL)
	{
		std::string msg = ":ft_irc 442 " + cli.getNickName() + " " + channel_name + " :You're not on that channel";
		cli.sendMessageOnClientFd(msg);
		return; //! Client not in channel
	}
	if (tokens.size() == 2)
	{
		topic = channel->getTopic();
		if (topic.empty())
		{
			std::string msg = ":ft_irc 331 " + cli.getNickName() + " " + channel_name + " :No topic is set";
			cli.sendMessageOnClientFd(msg);
		}
		else
		{
			std::string msg = ":ft_irc 332 " + cli.getNickName() + " " + channel_name + " :" + topic;
			cli.sendMessageOnClientFd(msg);
		}
		return;
	}
	if (tokens[2][0] != ':')
	{
		std::string msg = ":ft_irc 461 " + cli.getNickName() + " TOPIC :Not enough parameters";
		cli.sendMessageOnClientFd(msg);
		return; //!
	}
	if (channel->isTopicOpOnly() && !channel->isOperator(cli.getNickName()))
	{
		std::string msg = ":ft_irc 482 " + cli.getNickName() + " " + channel_name + " :You're not channel operator";
		cli.sendMessageOnClientFd(msg);
		return; //! Client not operator
	}
	topic = tokens[2].substr(1);
	for (size_t i = 3; i < tokens.size(); i++)
		topic += tokens[i];
	channel->setTopic(topic);
	std::string msg = ":" + cli.getPrefix() + " TOPIC " + channel_name + " :" + topic;
	channel->sendChannelMessage(cli, msg);
	cli.sendMessageOnClientFd(msg);
}

// static std::vector<std::string> GetTokens(std::string cpy, std::string &final)
// {
// 	size_t colonPos = cpy.find(":");
// 	if (colonPos == std::string::npos)
// 		return split(cpy, " ");
// 	final = cpy.substr(colonPos + 1);
// 	std::string header = cpy.substr(0, colonPos);
// 	return split(header, " ");
// }

void Server::cmdPrivMsg(Client &myClient, std::string cmd)
{

	std::string bufferCpy = cmd;
	std::vector<std::string> tokens = split(cmd, ' ');
	if (tokens.size() < 2)
	{
		std::string msg = ":ft_irc 411 " + myClient.getNickName() + " :No recipient given (" + cmd + ")";
		myClient.sendMessageOnClientFd(msg);
		return;
	}
	if (tokens.size() < 3 || tokens[2][0] != ':' || tokens[2].size() == 1)
	{
		std::string msg = ":ft_irc 412 " + myClient.getNickName() + " :No text to send";
		myClient.sendMessageOnClientFd(msg);
		return;
	}
	std::string message = tokens[2].substr(1);
	for (size_t i = 3; i < tokens.size(); i++)
		message += tokens[i];
	std::string target = tokens[1];
	std::string formattedMsg = ":" + myClient.getPrefix() + " PRIVMSG " + target + " :" + message;
	// std::cout << "Message send looks like this -> " << formattedMsg << std::endl;
	if (target[0] == '#')
	{
		if (getChannel(target).getClient(myClient.getNickName()) == NULL)
		{
			std::string msg = ":ft_irc 442 " + myClient.getNickName() + " " + target + " :You're not on that channel";
			myClient.sendMessageOnClientFd(msg);
			return; //! Client not in channel
		}
		try
		{
			getChannel(target).sendChannelMessage(myClient, formattedMsg);
		}
		catch (std::exception &e)
		{
			std::string msg = ":ft_irc 401 " + myClient.getNickName() + " " + target + " :No such channel";
			myClient.sendMessageOnClientFd(msg);
		}
	}
	else
	{
		int targetFd = findFdByNickName(target);
		if (targetFd != -1)
			findClientByFd(targetFd)->sendMessageOnClientFd(formattedMsg);
		else
		{
			std::string msg = ":ft_irc 401 " + myClient.getNickName() + " " + target + " :No such nick";
			myClient.sendMessageOnClientFd(msg);
		}
	}
}