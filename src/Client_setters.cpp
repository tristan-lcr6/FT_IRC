/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client_setters.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jferrand <jferrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 13:25:21 by jferrand          #+#    #+#             */
/*   Updated: 2026/03/06 13:25:22 by jferrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

int Client::setFd(int fd)
{
	return (this->_fd = fd);
}

std::string Client::setIp(std::string str)
{
	return (this->_IP = str);
}

std::string Client::setBuffer(std::string str)
{
	this->_buff = str;
	return (_buff);
}

void Client::setGrade(int i)
{
	this->_authStatus = i;
}

void Client::setToClean(int setVal)
{
	_toClean = setVal;
}

void Client::setNickName(std::string name)
{
	this->_nickName = name;
}

void Client::setRealName(std::string name)
{
	this->_realName = name;
}

void Client::setUserName(std::string name)
{
	this->_userName = name;
}
