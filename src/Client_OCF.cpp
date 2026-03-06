/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client_OCF.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jferrand <jferrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 13:25:09 by jferrand          #+#    #+#             */
/*   Updated: 2026/03/06 13:25:11 by jferrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : _authStatus(0),  _toClean(0), _nickName("*")
{
	return ;
}

Client::Client(int fd) : _fd(fd), _authStatus(0),   _toClean(0), _nickName("*")
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
		this->_IP = other.getIp();
		this->_toClean = other._toClean;
	}
	return (*this);
}

Client::~Client(void)
{
	// std::cout << "Destructor called" << std::endl;
	return ;
}
