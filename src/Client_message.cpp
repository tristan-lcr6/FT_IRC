#include "Client.hpp"

void Client::sendMessageOnClientFd(std::string msg) const
{
	if (this->_fd < 0)
		return;
	msg += "\r\n";
	if (send(this->_fd, msg.c_str(), msg.length(), 0) == -1)
		throw ClientException("Error while sending to client : " + _nickName);
}