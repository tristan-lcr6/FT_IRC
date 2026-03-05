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
	_bot.handleMessage(this, message);
}


void Channel::sendChannelMessageBot(std::string message)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
			_clients[i]->sendMessageOnClientFd(message);
	}
}