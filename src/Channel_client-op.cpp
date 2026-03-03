#include "Channel.hpp"

void Channel::addOperator(Client *cli)
{
	this->_operators.push_back(cli);
}

void Channel::addOperator(std::string nick)
{
	Client *cli = this->getClient(nick);
	this->addOperator(cli);
}

void Channel::removeOperator(Client *cli)
{
	for (std::size_t i = 0; i < this->_operators.size(); i++)
	{
		if (*this->_operators[i] == *cli)
		{
			this->_operators.erase(this->_operators.begin() + i);
			return;
		}
	}
}

void Channel::removeOperator(std::string nick)
{
	Client *cli = this->getClient(nick);
	this->removeOperator(cli);
}

Client *Channel::getClient(std::string nick)
{
	for (std::size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i]->getNickName() == nick)
			return (this->_clients[i]);
	}
	std::cerr << "Client " << nick << " not found in channel " << this->_name << std::endl;
	return (NULL); //! Error client not found j'arrive pas a renvoyer NULL
}

void Channel::join(Client &cli)
{
	if (this->_k_mode)
	{
		std::string msg = ":ft_irc 475 " + cli.getNickName() + " " + this->_name + " :Cannot join channel (+k)";
		cli.sendMessageOnClientFd(msg);
		return ; //! erreur besoin de password
	}
	if (this->_l_mode && this->_clients.size() >= this->_client_limit)
	{
		std::string msg = ":ft_irc 471 " + cli.getNickName() + " " + this->_name + " :Cannot join channel (+l)";
		cli.sendMessageOnClientFd(msg);
		return ; //! erreur peut pas rejoindre
	}
	if (this->_i_mode)
	{
		for (std::size_t i = 0; i < this->_invite_list.size(); i++)
		{
			if (*this->_invite_list[i] == cli)
			{
				this->_clients.push_back(&cli);
				return ;
			}
		}
		std::string msg = ":ft_irc 473 " + cli.getNickName() + " " + this->_name + " :Cannot join channel (+i)";
		cli.sendMessageOnClientFd(msg);
		return ; //! erreur n'est pas invite
	}
	this->_clients.push_back(&cli);
}

void Channel::join(Client &cli, std::string pwd)
{
	if (this->_k_mode && this->_password != pwd)
	{
		std::string msg = ":ft_irc 475 " + cli.getNickName() + " " + this->_name + " :Cannot join channel (+k)";
		cli.sendMessageOnClientFd(msg);
		return ; //! erreur mauvais password
	}
	if (this->_l_mode && this->_clients.size() >= this->_client_limit)
	{
		std::string msg = ":ft_irc 471 " + cli.getNickName() + " " + this->_name + " :Cannot join channel (+l)";
		cli.sendMessageOnClientFd(msg);
		return ; //! erreur peut pas rejoindre
	}
	if (this->_i_mode)
	{
		for (std::size_t i = 0; i < this->_invite_list.size(); i++)
		{
			if (*this->_invite_list[i] == cli)
			{
				this->_clients.push_back(&cli);
				return ;
			}
		}
		std::string msg = ":ft_irc 473 " + cli.getNickName() + " " + this->_name + " :Cannot join channel (+i)";
		cli.sendMessageOnClientFd(msg);
		return ; //! erreur n'est pas invite
	}
	this->_clients.push_back(&cli);
}

void Channel::kick(Client &cli)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (*this->_clients[i] == cli)
		{
			this->_clients.erase(this->_clients.begin() + i);
			break;
		}
	}
	for (size_t i = 0; i < this->_invite_list.size(); i++)
	{
		if (*this->_invite_list[i] == cli)
		{
			this->_invite_list.erase(this->_invite_list.begin() + i);
			break;
		}
	}
	this->removeOperator(&cli);
}

