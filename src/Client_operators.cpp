#include "Client.hpp"

bool Client::operator==(const Client &c)
{
	if (this->_fd == c._fd)
		return (true);
	return (false);
}

std::ostream &operator<<(std::ostream &os, const Client &c)
{
	os << "======= [Client Info] =======" << std::endl;
	os << "FD          : " << c.getFd() << std::endl;
	os << "IP          : " << c.getIp() << std::endl;
	os << "Nickname    : " << (c.getNickName().empty() ? "(none)" : c.getNickName()) << std::endl;
	os << "UserName    : " << (c.getUserName().empty() ? "(none)" : c.getUserName()) << std::endl;
	os << "Realname    : " << (c.getRealName().empty() ? "(none)" : c.getRealName()) << std::endl;
	os << "Auth Status : " << c.getAuthStatus() << std::endl;
	os << "Buffer      : " << c.getBuffer() << std::endl;
	os << "=============================";
	return (os);
}

bool Client::operator!=(const Client &other)
{
	if(this->_fd != other._fd)
		return (true);
	return (false);
}

