

#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "utils.hpp"

std::vector<std::string> GetTokens(std::string cpy, std::string &final)
{
	size_t colonPos = cpy.find(":");
	if (colonPos == std::string::npos)
		return split(cpy, " ");
	final = cpy.substr(colonPos + 1);
	std::string header = cpy.substr(0, colonPos);
	return split(header, " ");
}

void Server::cmdPrivMsg(Client &myClient)
{

	std::string message;
	std::string channelName;

	std::string bufferCpy = myClient.getBuffer();

	std::vector<std::string> tokens = GetTokens(bufferCpy, message);

	if (tokens.size() < 2)
	{
		std::cout << "Error: No target specified" << std::endl;
		return;
	}

	std::string target = tokens[1];
	std::string formattedMsg = ":" + myClient.getNickName() + " PRIVMSG " + target + " :" + message + "\r\n";
	// std::cout << "Message send looks like this -> " << formattedMsg << std::endl;
	if (target[0] == '#')
	{
		channelName = target.substr(1);
		try
		{
			getChannel(channelName).sendChannelMessage(myClient, formattedMsg);
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	else
	{
		try
		{

			int targetFd = findFdByNickName(target);
			if (targetFd != -1)
				findClientByFd(targetFd).sendMessageOnClientFd(formattedMsg);
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}

Channel &Server::getChannel(std::string Channelname)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		std::cout << _channels[i] << std::endl;//!
		if (_channels[i]->getName() == Channelname)
		{
			return (*_channels[i]);
		}
	}
	throw ServerException("!Error :could not find channel named  : " + Channelname); //! throw exception instead
}

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