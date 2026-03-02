#include "Server.hpp"


void Server::JoinMessage(std::string channelName, Client &cli)
{
cli.sendMessageOnClientFd(cli.getPrefix() + " JOIN " + channelName);
cli.sendMessageOnClientFd(cli.getPrefix() + " 332 " + channelName " :Bienvenue sur " + channelName);
    

    // 3. Envoyer l'info du Topic (RPL_TOPICWHOTIME 333 - Optionnel mais Irssi adore)
    cli.reply(":" + serverName + " 333 " + nick + " " + channelName + " " + nick + " 1672531200");

    // 4. Envoyer la liste des noms (RPL_NAMREPLY 353)
    // Ici tu dois boucler sur tous les membres du channel pour construire la liste
    std::string list = "@" + nick + " member2 member3"; 
    cli.reply(":" + serverName + " 353 " + nick + " = " + channelName + " :" + list);

    // 5. Finir la liste (RPL_ENDOFNAMES 366) - CRUCIAL
    cli.reply(":" + serverName + " 366 " + nick + " " + channelName + " :End of /NAMES list");
}



// JOIN #a,#b,#c passA,passB,passC
// If the channel exists and can join, joins it
// If it doesn't exist create it
void Server::cmdJoin(Client &cli)
{
	std::string pass;
	std::vector<std::string> tokens = split(cli.getBuffer(), ' ');
	if (tokens.size() < 2)
		return;
	std::vector<std::string> channels = split(tokens[1], ',');
	std::vector<std::string> passwords;
	if (tokens.size() > 2)
		passwords = split(tokens[2], ',');
	for (std::size_t i = 0; i < channels.size(); i++)
	{

		std::string name = channels[i];
		if(name[0] != '#')
			throw ServerException("Cannot find '#' to start channel name.");
		bool found = false;
		for (std::size_t j = 0; j < this->_channels.size(); j++)
		{
			
			if (name == this->_channels[j]->getName())
			{
				if (i < passwords.size())
					pass = passwords[i];
				else
					pass = ""; //!change password handling ?
				this->_channels[j]->join(cli, pass);
				std::cout << "join existing channel" << *(this->_channels[j]) << std::endl;
				
				found = true;
				break;
			}
		}
		if (!found)
		{
			Channel *newChan = new Channel(cli, name);
			this->_channels.push_back(newChan);
			// std::cout << "Channel " << name << " created and client added." << std::endl;
			std::cout << *newChan << std::endl;
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
	if (!this->isAlreadyChannel(channel, channel_name))
	{
		std::cerr << "Error: tried to mode an unknown channel: " << channel_name << std::endl;
		return; //! Channel doesn't exist
	}
	std::string modestring = tokens[2];
	std::vector<std::string> params;
	if (tokens.size() > 3)
		params.assign(tokens.begin() + 3, tokens.end());
	;
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
				return; //! error not enough params
			}
			channel->applyMode(c, add, params[paramIdx++]);
		}
		else
			channel->applyMode(c, add);
	}
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
	if (!this->isAlreadyChannel(channel, channel_name))
	{
		std::cerr << "Error: tried to kick in an unknown channel: " << channel_name << std::endl;
		return ; //! Channel doesn't exist
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
	if (!this->isAlreadyChannel(channel, channel_name))
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
	if (!this->isAlreadyChannel(channel, channel_name))
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





static std::vector<std::string> GetTokens(std::string cpy, std::string &final)
{
	size_t colonPos = cpy.find(":");
	if (colonPos == std::string::npos)
		return split(cpy, " ");
	final = cpy.substr(colonPos + 1);
	std::string header = cpy.substr(0, colonPos);
	return split(header, " ");
}

void Server::cmdPrivMsg(Client &myClient)
{

	std::string message;
	std::string channelName;
	std::string bufferCpy = myClient.getBuffer();
	std::vector<std::string> tokens = GetTokens(bufferCpy, message);
	if (tokens.size() < 2)
	{
		std::cout << "Error: No target specified" << std::endl;
		return;
	}
	std::string target = tokens[1];
	std::string formattedMsg = ":" + myClient.getNickName() + " PRIVMSG " + target + " :" + message + "\r\n";
	// std::cout << "Message send looks like this -> " << formattedMsg << std::endl;
	if (target[0] == '#')
	{
		channelName = target.substr(1);
		try
		{
			getChannel(channelName).sendChannelMessage(myClient, formattedMsg);
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	else
	{
		try
		{

			int targetFd = findFdByNickName(target);
			if (targetFd != -1)
				findClientByFd(targetFd).sendMessageOnClientFd(formattedMsg);
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}