/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jferrand <jferrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:22:56 by jferrand          #+#    #+#             */
/*   Updated: 2026/02/24 17:50:25 by jferrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <arpa/inet.h> //-> for inet_ntoa()
# include <csignal>     //-> for signal()
# include <cstdio>     //-> for signal()
# include <fcntl.h>     //-> for fcntl()
# include <iostream>
# include <netinet/in.h> //-> for sockaddr_in
# include <poll.h>       //-> for poll()
# include <sys/socket.h> //-> for socket()
# include <sys/types.h>  //-> for socket()
# include <unistd.h>     //-> for close()
# include <vector>       //-> for vector

# define BLACK "\033[90m"
# define RED "\033[91m"
# define GREEN "\033[92m"
# define YELLOW "\033[93m"
# define BLUE "\033[94m"
# define PURPLE "\033[95m"
# define CYAN "\033[96m"
# define END "\033[0m"

class Server;

#endif