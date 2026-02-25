#include "Client.hpp"

Client::Client()
{
	return ;
}

Client::Client(int fd) : _fd(fd)
{
	std::cout << "Client constructor called" << std::endl;
	return ;
}

Client::Client(const Client &other)
{
	std::cout << "Copy constructor called" << std::endl;
	if (this != &other)
		*this = other;
	return ;
}

Client &Client::operator=(const Client &other)
{
	std::cout << "Assignment operator called" << std::endl;
	if (this != &other)
	{
		this->_fd = other.getFd();
	}
	return (*this);
}

Client::~Client(void)
{
	std::cout << "Destructor called" << std::endl;
	return ;
}

int Client::getFd() const
{
	return (this->_fd);
}

int Client::setFd(int fd)
{
	return (this->_fd = fd);
}

std::string Client::getIp(void) const
{
	return (this->_IP);
}

std::string Client::setIp(std::string str)
{
	return (this->_IP = str);
}

void Client::addBuff(std::string data)
{
	this->_buff += data;
}