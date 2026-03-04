#include "Channel.hpp"

bool Channel::isInviteOnly(void) const
{
	return (this->_i_mode);
}

bool Channel::isTopicOpOnly(void) const
{
	return (this->_t_mode);
}

bool Channel::hasKey(void) const
{
	return (this->_k_mode);
}

bool Channel::isLimited(void) const
{
	return (this->_l_mode);
}

const std::string &Channel::getTopic(void) const
{
	return (this->_topic);
}

const std::string &Channel::getKey(void) const
{
	return (this->_password);
}

const std::string &Channel::getTopicWhoTime(void) const
{
	return (this->_topicwhotime);
}

size_t Channel::getLimit(void)
{
	return (this->_client_limit);
}

std::string Channel::getClientList(void) const
{
	std::string list;
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (i != 0)
			list += " ";
		std::string nick = this->_clients[i]->getNickName();
		if (this->isOperator(nick))
			list += "@";
		list += nick;
	}
	return list;
}

size_t Channel::getClientsSize(void) 
{
	return (this->_clients.size());
}

const std::string &Channel::getName(void) const
{
	return this->_name;
}

bool Channel::isOperator(std::string nick) const
{
	for (std::size_t i = 0; i < this->_operators.size(); i++)
	{
		if (this->_operators[i]->getNickName() == nick)
			return (true);
	}
	return (false);
}
