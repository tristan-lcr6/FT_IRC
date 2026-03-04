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

size_t Channel::getClientsSize(void) 
{
	return (this->_clients.size());
}

const std::string &Channel::getName(void) const
{
	return this->_name;
}

bool Channel::isOperator(std::string nick)
{
	for (std::size_t i = 0; i < this->_operators.size(); i++)
	{
		if (this->_operators[i]->getNickName() == nick)
			return (true);
	}
	return (false);
}
