

#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "utils.hpp"

std::vector<std::string> GetTokens(std::string &cpy)
{
	std::string message;
	std::string formattedMsg;
	if(cpy.find(":") == std::string::npos)
		std::cout << "Error :could not find ':' to start collect message" << std::endl;
	message = cpy.substr(cpy.find(":") + 1);
	cpy.erase(cpy.find(":") - 1);
	cpy = message;
	return (split(cpy, " "));
}

void Server::cmdPrivMsg(Client &myClient)
{
	std::string target;
	std::string cpy = myClient.getBuffer();
	std::vector<std::string> tokens = GetTokens(cpy);
	target = tokens[1];
	std::string formattedMsg = ":" + myClient.getNickName() + " PRIVMSG " + target + " :" +  + "\n";
	std::cout << "Message send looks like this->" << formattedMsg << std::endl;
	if (tokens.size() != 2)
	{
		std::cout << "Error :Not a valid entry for Private Message." << std::endl;
		return ;
	}
	if (target[0] == '#') 
	{
        try 
		{
      	      getChannel(target).sendChannelMessage(myClient, formattedMsg);
      	}
		catch (std::exception &e)
		{
				std::cout << e.what() << std::endl;
		}
    } 
	else 
	{
        int targetFd = findFdByNickName(target);
        if (targetFd != -1) 
		{
            findClientByFd(targetFd).sendMessageOnClientFd(formattedMsg);
        }
	}
}

Channel &Server::getChannel(std::string Channelname)
{
	for (size_t i = 0; i < _channels.size();i++)
	{
		if(_channels[i].getName() == Channelname)
		{
			return (_channels[i]);
		}
	}
	throw ServerException("!Error :could not find channel named  : " + Channelname); //! throw exception instead
}

void Client::sendMessageOnClientFd(std::string& msg) const 
{
    if (this->_fd < 0)
		 return;
    if (send(this->_fd, msg.c_str(), msg.length(), 0) == -1) 
	{
        std::cerr << "Error while sending to client." << _nickName << std::endl;
    }
}

void Channel::sendChannelMessage(Client & myClient, std::string message)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if(_clients[i] != myClient)
			_clients[i].sendMessageOnClientFd(message);	
	}
}