#include "Channel.hpp"

// Default constructor
Channel::Channel(void) : _name("#default"), _i_mode(false), _t_mode(false), _k_mode(false), _l_mode(false)
{
	// std::cout << "Default constructor called" << std::endl;
	return;
}

Channel::Channel(Client &cli, std::string name) : _name(name), _i_mode(false), _t_mode(false), _k_mode(false), _l_mode(false)
{
	this->_operators.push_back(&cli);\
	this->_clients.push_back(&cli);
	//! need to be add to others vectors
	return;
}

// Copy constructor
Channel::Channel(const Channel &other)
{
	// std::cout << "Copy constructor called" << std::endl;
	this->_name = other._name;
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
	return;
}

// Assignment operator overload
Channel &Channel::operator=(const Channel &other)
{
	// std::cout << "Assignment operator called" << std::endl;
	this->_name = other._name;
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
	return;
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
	this->_i_mode = b;
}

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

void Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}

const std::string &Channel::getName(void) const
{
	return this->_name;
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
		std::cerr << "Error: tried to join " << this->_name << " without password" << std::endl;
		return ; //! erreur besoin de password
	}
	if (this->_l_mode && this->_clients.size() >= this->_client_limit)
	{
		std::cerr << "Error: can't join, " << this->_name << " is full" << std::endl;
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
		std::cerr << "Error: Can't join " << this->_name << " because you (" << cli.getNickName() << ") are not invited" << std::endl;
		return ; //! erreur n'est pas invite
	}
	this->_clients.push_back(&cli);
}

void Channel::join(Client &cli, std::string pwd)
{
	if (this->_k_mode && this->_password != pwd)
	{
		std::cerr << "Error: tried to join " << this->_name << " with wrong password" << std::endl;
		return ; //! erreur mauvais password
	}
	if (this->_l_mode && this->_clients.size() >= this->_client_limit)
	{
		std::cerr << "Error: can't join, " << this->_name << " is full" << std::endl;
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
		std::cerr << "Error: Can't join " << this->_name << " because you (" << cli.getNickName() << ") are not invited" << std::endl;
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

void Channel::invite(Client &cli)
{
	this->_invite_list.push_back(&cli);
}

bool Channel::modeWithParam(char c, bool add)
{
	if (add)
	{
		std::string modes = "kol";
		if (modes.find(c) != std::string::npos)
			return true;
		return false;
	}
	if (c == 'o')
		return true;
	return false;
}

// void Channel::applyMode(char c, bool add)
// {
// 	if (modeWithParam(c, add))
// 	{
// 		std::cerr << "Error: tried to apply mode " << c << " without params" << std::endl;
// 		return; //! error mode needs param
// 	}
// 	switch (c)
// 	{
// 	case 'i':
// 		this->setInviteOnly(add);
// 		break;
// 	case 't':
// 		this->setTopicOpOnly(add);
// 		break;
// 	case 'k':
// 		this->removePassword();
// 		break;
// 	case 'l':
// 		this->removeClientLimit();
// 		break;
// 	default:
// 		std::cerr << "Error: tried to apply an unknown mode: " << c << std::endl;
// 		//! error unknown mode
// 		break;
// 	}
// }

void Channel::applyMode(char c, bool add, std::string param)
{
	switch (c)
	{
	case 'k':
		this->setPassword(param);
		break;
	case 'o':
		if (add)
			this->addOperator(param);
		else
			this->removeOperator(param);
		break;
	case 'l':
		this->setClientLimit(param);
		break;
	default:
		std::cerr << "Error: tried to apply an unknown mode: " << c << std::endl;
		//! error unknown mode
		break;
	}
}

std::ostream &operator<<(std::ostream &os, const Channel &channel)
{
	os << "========== CHANNEL DEBUG ==========" << std::endl;
	os << "Name     : " << channel.getName() << std::endl;
	os << "Topic    : " << channel.getTopic() << std::endl;
	os << "Modes    : "
	   << (channel.isInviteOnly() ? "i" : "")
	   << (channel.isTopicOpOnly() ? "t" : "")
	   << (channel.hasKey() ? "k" : "")
	   << (channel.isLimited() ? "l" : "") << std::endl;

	os << "Clients (Addresses) [" << channel._clients.size() << "]: " << std::endl;
	for (size_t i = 0; i < channel._clients.size(); ++i)
	{
		os << "  - " << channel._clients[i] << " (Nick: " << channel._clients[i]->getNickName() << ")" << std::endl;
	}

	os << "Operators (Addresses) [" << channel._operators.size() << "]: " << std::endl;
	for (size_t i = 0; i < channel._operators.size(); ++i)
	{
		os << "  - " << channel._operators[i] << std::endl;
	}

	os << "Invite List (Addresses) [" << channel._invite_list.size() << "]: " << std::endl;
	for (size_t i = 0; i < channel._invite_list.size(); ++i)
	{
		os << "  - " << channel._invite_list[i] << std::endl;
	}

	os << "====================================";
	return os;
}
