#include "Client.hpp"

void Client::addBuff(std::string data)
{
	this->_buff += data;
}

void Client::clearBuffer()
{
	this->_buff.clear();
}
