#include "Channel.hpp"

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

void Channel::applyMode(Client &cli, char c, bool add)
{
	if (modeWithParam(c, add))
	{
		std::string msg = ":ft_irc 461 " + cli.getNickName() + " MODE :Not enough parameters";
		cli.sendMessageOnClientFd(msg);
		return; //! error mode needs param
	}
	switch (c)
	{
	case 'i':
		this->setInviteOnly(add);
		break;
	case 't':
		this->setTopicOpOnly(add);
		break;
	case 'k':
		this->removePassword();
		break;
	case 'l':
		this->removeClientLimit();
		break;
	default:
		std::string msg = ":ft_irc 472 " + cli.getNickName() + " " + c + " :is unknown mode char to me";
		cli.sendMessageOnClientFd(msg);
		//! error unknown mode
		return;
	}
	std::string msg = ":" + cli.getPrefix() + " MODE " + this->_name + " ";
	if (add)
		msg += "+";
	else
		msg += "-";
	msg.push_back(c);
	cli.sendMessageOnClientFd(msg);
	this->sendChannelMessage(cli, msg);
}

void Channel::applyMode(Client &cli, char c, bool add, std::string param)
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
		std::string msg = ":ft_irc 472 " + cli.getNickName() + " ";
		msg.push_back(c);
		msg += " :is unknown mode char to me";
		cli.sendMessageOnClientFd(msg);
		//! error unknown mode
		return;
	}
	std::string msg = ":" + cli.getPrefix() + " MODE " + this->_name + " ";
	if (add)
		msg += "+";
	else
		msg += "-";
	msg.push_back(c);
	msg += " " + param;
	cli.sendMessageOnClientFd(msg);
	this->sendChannelMessage(cli, msg);
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
