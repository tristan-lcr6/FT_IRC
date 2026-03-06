/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client_operators.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jferrand <jferrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 13:25:12 by jferrand          #+#    #+#             */
/*   Updated: 2026/03/06 13:25:18 by jferrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

bool Client::operator==(const Client &c)
{
	if (this->_fd == c._fd)
		return (true);
	return (false);
}

bool Client::operator!=(const Client &other)
{
	if(this->_fd != other._fd)
		return (true);
	return (false);
}
