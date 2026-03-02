#include "Server.hpp"

bool Server::_signal = false;

// Default constructor
Server::Server(void) : _port(-1), _serSocketFd(-1), _password("")
{
	// std::cout << "Default constructor called" << std::endl;
	return;
}

// Copy constructor
Server::Server(const Server &other) : _port(other._port),
									  _serSocketFd(other._serSocketFd), _clients(other._clients), _fds(other._fds)
{
	// std::cout << "Copy constructor called" << std::endl;
	return;
}

// Assignment operator overload
Server &Server::operator=(const Server &other)
{
	// std::cout << "Assignment operator called" << std::endl;
	this->_port = other._port;
	this->_serSocketFd = other._serSocketFd;
	this->_clients = other._clients;
	this->_fds = other._fds;
	return (*this);
}

// Destructor
Server::~Server(void)
{
	// std::cout << "Destructor called" << std::endl;
	return;
}


bool isValidString(const std::string &str)
{
	char c;

	if (str.empty())
		return (false);
	for (size_t i = 0; i < str.length(); ++i)
	{
		c = str[i];
		if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'))
		{
			return (false);
		}
	}
	return (true);
}


std::ostream &operator<<(std::ostream &dataStream, const std::vector<std::string> &vector)
{
	dataStream << "[";
	for (size_t i = 0; i < vector.size(); ++i)
	{
		dataStream << vector[i];
		if (i != vector.size() - 1)
		{
			dataStream << ", ";
		}
	}
	dataStream << "]";
	return dataStream;
}
