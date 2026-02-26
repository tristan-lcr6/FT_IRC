#include "Channel.hpp"

// Default constructor
Channel::Channel(void)
{
	// std::cout << "Default constructor called" << std::endl;
	return ;
}

Channel::Channel(Client &cli)
{
	this->_operators.push_back(cli);
	return ;
}

// Copy constructor
Channel::Channel(const Channel &other)
{
	// std::cout << "Copy constructor called" << std::endl;
	this->_topic = other._topic;
	this->_clients = other._clients;
	this->_i_mode = other._i_mode;
	this->_invite_list = other._invite_list;
	this->_t_mode = other._t_mode;
	this->_k_mode = other._k_mode;
	this->_password = other._password;
	this->_operators = other._operators;
	this->_l_mode = other._l_mode;
	this->_client_limit = other._client_limit;
	return ;
}

// Assignment operator overload
Channel &Channel::operator=(const Channel &other)
{
	// std::cout << "Assignment operator called" << std::endl;
	this->_topic = other._topic;
	this->_clients = other._clients;
	this->_i_mode = other._i_mode;
	this->_invite_list = other._invite_list;
	this->_t_mode = other._t_mode;
	this->_k_mode = other._k_mode;
	this->_password = other._password;
	this->_operators = other._operators;
	this->_l_mode = other._l_mode;
	this->_client_limit = other._client_limit;
	return (*this);
}

// Destructor
Channel::~Channel(void)
{
	// std::cout << "Destructor called" << std::endl;
	return ;
}

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

void Channel::setInviteOnly(bool b)
{
	b += 1;//!

	if (this->_i_mode)
		return ;
	this->_invite_list.clear();
	this->_i_mode = true;
}

void Channel::setTopicOpOnly(bool b)
{
	b += 1 ;//!
	this->_t_mode = true;
}

void Channel::setClientLimit(int limit)
{
	this->_l_mode = true;
	this->_client_limit = limit;
}

void Channel::removeClientLimit(int limit)
{
	limit++;//!
	this->_l_mode = false;
}

void Channel::addOperator(Client &cli)
{
	this->_operators.push_back(cli);
}

void Channel::removeOperator(Client &cli)
{
	for (std::size_t i = 0; i < this->_operators.size(); i++)
	{
		if (this->_operators[i] == cli)
		{
			this->_operators.erase(this->_operators.begin() + i);
			return ;
		}
	}
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

const std::string &Channel::getTopic(void) const
{
	return (this->_topic);
}

void Channel::join(Client &cli)
{
	// if (this->_l_mode && this->_clients.size() >= this->_client_limit)//!
	// 	return ; //! erreur peut pas rejoindre
	if (this->_i_mode)
	{
		for (std::size_t i = 0; i < this->_invite_list.size(); i++)
		{
			if (this->_invite_list[i] == cli)
			{
				this->_clients.push_back(cli);
				return ;
			}
		}
		return ; //! erreur n'est pas invite
	}
	this->_clients.push_back(cli);
}

void Channel::invite(Client &cli)
{
	this->_invite_list.push_back(cli);
}
