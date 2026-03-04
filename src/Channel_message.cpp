
#include "Channel.hpp"

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