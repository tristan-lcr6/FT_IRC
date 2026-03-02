#include "Channel.hpp"

void Channel::setTopicOpOnly(bool b)
{
	this->_t_mode = b;
}

void Channel::setClientLimit(size_t limit)
{
	this->_l_mode = true;
	this->_client_limit = limit;
}

void Channel::setClientLimit(std::string limit_str)
{
	char *end;
	long l = std::strtol(limit_str.c_str(), &end, 10);
	if (*end != '\0' || l < 1 )
	{
		std::cerr << "Invalid client limit" << std::endl;
		return; //! error invalid input
	}
	this->setClientLimit(static_cast<size_t>(l));
}

void Channel::removeClientLimit()
{
	this->_l_mode = false;
}

void Channel::setPassword(std::string psw)
{
	this->_k_mode = true;
	this->_password = psw;
}

void Channel::removePassword(void)
{
	this->_k_mode = false;
	this->_password.clear();
}

void Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}

void Channel::invite(Client &cli)
{
	this->_invite_list.push_back(&cli);
}

void Channel::setInviteOnly(bool b)
{
	this->_i_mode = b;
}