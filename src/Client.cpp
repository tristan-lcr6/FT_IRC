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

	}
	return (*this);
}

Client::~Client(void)
{
	// std::cout << "Destructor called" << std::endl;
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
std::string Client::getBuffer(void) const
{
	return (this->_buff);
}

std::string Client::setBuffer(std::string str)
{
	this->_buff = str;
	return (_buff);
}

void Client::clearBuffer()
{
	this->_buff.clear();
}

int Client::getAuthStatus() const
{
	return (this->_authStatus);
}

bool Client::operator==(const Client &c)
{
	if (this->_fd == c._fd)
		return (true);
	return (false);
}

void Client::setGrade(int i)
{
	this->_authStatus = i;
}

void Client::setNickName(std::string name)
{
	this->_nickName = name;
}

std::string Client::getNickName() const
{
	return (_nickName);
}

std::string Client::getUserName() const
{
	return (_userName);
}




std::ostream &operator<<(std::ostream &os, const Client &c)
{
	os << "======= [Client Info] =======" << std::endl;
	os << "FD          : " << c.getFd() << std::endl;
	os << "IP          : " << c.getIp() << std::endl;
	os << "Nickname    : " << (c.getNickName().empty() ? "(none)" : c.getNickName()) << std::endl;
	os << "Auth Status : " << c.getAuthStatus() << std::endl;
	os << "Buffer      : " << c.getBuffer() << std::endl;
	os << "=============================";
	return (os);
}