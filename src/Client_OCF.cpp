#include "Client.hpp"

Client::Client() : _authStatus(0)
{
	return ;
}

Client::Client(int fd) : _fd(fd), _authStatus(0)
{
	// std::cout << "Client constructor called" << std::endl;
	return ;
}

Client::Client(const Client &other)
{
	// std::cout << "Copy constructor called" << std::endl;
	if (this != &other)
		*this = other;
	return ;
}

Client &Client::operator=(const Client &other)
{
	// std::cout << "Assignment operator called" << std::endl;
	if (this != &other)
	{
		this->_fd = other.getFd();
		this->_authStatus = other.getAuthStatus();
		this->_nickName = other.getNickName();
		this->_userName = other.getUserName();
		this->_buff = other.getBuffer();
		this->_realName = other.getRealName();
	}
	return (*this);
}

Client::~Client(void)
{
	// std::cout << "Destructor called" << std::endl;
	return ;
}
