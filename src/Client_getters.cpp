#include "Client.hpp"

int Client::getFd() const
{
	return (this->_fd);
}

std::string Client::getIp(void) const
{
	return (this->_IP);
}

std::string Client::getNickName(void) const
{
	return (this->_nickName);
}

const std::string &Client::getBuffer(void) const
{
	return (this->_buff);
}

std::string &Client::getBuffer(void)
{
	return (this->_buff);
}

int Client::getAuthStatus() const
{
	return (this->_authStatus);
}

std::string Client::getUserName() const
{
	return (_userName);
}

std::string Client::getRealName() const
{
	return (_realName);
}

std::string Client::getPrefix() const
{
	std::string nickName = this->_nickName.empty() ? "Unnkown" : this->_nickName;
	std::string userName = this->_userName.empty() ? "Unnkown" : this->_userName;
	std::string IpAdd = this->_IP.empty() ? "Unnkown" : this->_IP;
    return nickName + "!" + userName + "@" + IpAdd;
}
