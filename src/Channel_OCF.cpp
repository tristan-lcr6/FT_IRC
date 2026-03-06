/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel_OCF.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jferrand <jferrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 13:24:55 by jferrand          #+#    #+#             */
/*   Updated: 2026/03/06 13:24:56 by jferrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Bot.hpp"

// Default constructor
Channel::Channel(void) : _name("#default"), _i_mode(false), _t_mode(false), _k_mode(false), _l_mode(false), _bot("SuperWeatherBot")
{
	// std::cout << "Default constructor called" << std::endl;
	return;
}

Channel::Channel(Client &cli, std::string name) : _name(name), _i_mode(false), _t_mode(false), _k_mode(false), _l_mode(false), _bot("SuperWeatherBot")
{
	this->_operators.push_back(&cli);
	this->_clients.push_back(&cli);
	//! need to be add to others vectors
	return;
}

// Copy constructor
Channel::Channel(const Channel &other)
{
	// std::cout << "Copy constructor called" << std::endl;
	this->_name = other._name;
	this->_topic = other._topic;
	this->_clients = other._clients;
	this->_i_mode = other._i_mode;
	this->_invite_list = other._invite_list;
	this->_t_mode = other._t_mode;
	this->_k_mode = other._k_mode;
	this->_password = other._password;
	this->_operators = other._operators;
	this->_l_mode = other._l_mode;
	this->_client_limit = other._client_limit;
	this->_bot = other._bot;
	return;
}

// Assignment operator overload
Channel &Channel::operator=(const Channel &other)
{
	// std::cout << "Assignment operator called" << std::endl;
	this->_name = other._name;
	this->_topic = other._topic;
	this->_clients = other._clients;
	this->_i_mode = other._i_mode;
	this->_invite_list = other._invite_list;
	this->_t_mode = other._t_mode;
	this->_k_mode = other._k_mode;
	this->_password = other._password;
	this->_operators = other._operators;
	this->_l_mode = other._l_mode;
	this->_client_limit = other._client_limit;
	this->_bot = other._bot;

	return (*this);
}

// Destructor
Channel::~Channel(void)
{
	// std::cout << "Destructor called" << std::endl;
	return;
}
