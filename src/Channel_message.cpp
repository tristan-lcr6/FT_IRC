/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel_message.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jferrand <jferrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 13:24:39 by jferrand          #+#    #+#             */
/*   Updated: 2026/03/06 13:24:40 by jferrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"

void Channel::sendChannelMessage(Client &myClient, std::string message)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i] != &myClient)
		{
			_clients[i]->sendMessageOnClientFd(message);
		}
	}
}

void Channel::sendChannelMessageBot(std::string message)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		_clients[i]->sendMessageOnClientFd(message);
	}
}
