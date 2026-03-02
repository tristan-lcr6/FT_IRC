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

const std::string &Channel::getName(void) const
{
	return this->_name;
}
