/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client_buffer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jferrand <jferrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 13:24:58 by jferrand          #+#    #+#             */
/*   Updated: 2026/03/06 13:25:00 by jferrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void Client::addBuff(std::string data)
{
	this->_buff += data;
}

void Client::clearBuffer()
{
	this->_buff.clear();
}
