/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_data_managment.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jferrand <jferrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 13:23:29 by jferrand          #+#    #+#             */
/*   Updated: 2026/03/06 13:23:38 by jferrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::setPort(int port)
{
	this->_port = port;
}

void Server::setPassword(std::string pswd)
{
	this->_password = pswd;
}

Client *Server::findClientByFd(int fd)
{
	std::vector<Client *>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		if ((*it)->getFd() == fd)
			return (*it);
	}
	return (NULL);
}

int Server::findFdByNickName(std::string nickName)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i]->getNickName() == nickName)
			return (_clients[i]->getFd());
	}
	return (-1);
}

Channel &Server::getChannel(std::string Channelname)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i]->getName() == Channelname)
		{
			return (*_channels[i]);
		}
	}
	throw ServerException("!Error :could not find channel named  : " + Channelname); //! throw exception instead
}

bool Server::isAlreadyChannel(Channel **channel, const std::string &channel_name)
{
	for (std::size_t i = 0; i < this->_channels.size(); i++)
	{
		if (channel_name == this->_channels[i]->getName())
		{
			*channel = this->_channels[i];
			return true;
		}
	}
	return false;
}
