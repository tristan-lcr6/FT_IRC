#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "utils.hpp"

void Client::sendMessageOnClientFd(std::string &msg) const
{
	if (this->_fd < 0)
		return;
	if (send(this->_fd, msg.c_str(), msg.length(), 0) == -1)
		throw ClientException("Error while sending to client : " + _nickName);
}

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