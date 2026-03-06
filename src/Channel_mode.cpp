/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel_mode.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jferrand <jferrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 13:24:44 by jferrand          #+#    #+#             */
/*   Updated: 2026/03/06 13:24:52 by jferrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"

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
