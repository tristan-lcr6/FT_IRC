
#include "Channel.hpp"

void Channel::sendChannelMessage(Client &myClient, std::string message)
{
	std::cout << "vector of client in this channel is size =" << _clients.size() << std::endl;
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i] != &myClient)
		{
			_clients[i]->sendMessageOnClientFd(message);
		}
	}
}