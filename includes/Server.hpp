/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlecuyer <tlecuyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:22:56 by jferrand          #+#    #+#             */
/*   Updated: 2026/02/25 18:59:15 by tlecuyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Client.hpp"
# include <arpa/inet.h> //-> for inet_ntoa()
# include <csignal>     //-> for signal()
# include <cstdio>      //-> for signal()
# include <cstdlib>
# include <cstring>
# include <fcntl.h> //-> for fcntl()
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

class Server
{
  private:
	int _port;
	int _serSocketFd;
	static bool _signal;
	std::string _password;
	std::vector<Client> _clients;
	std::vector<struct pollfd> _fds;

  public:
	Server(void);
	Server(const Server &other);
	Server &operator=(const Server &other);
	~Server();

	void serverInit();
	void serSocket();
	void acceptNewClient();
	void receiveNewData(int fd);
	void addToBuff(std::string data, Client &myClient);

	static void signalHandler(int signum);

	void setPort(int port);
	void setPassword(std::string pswd);

	void closeFds();
	void clearClient(int fd);

	Client &findClientByFd(int fd);
	void execute(Client cli);
	int cmdPass(Client &myClient);
	int cmdNick(Client &myClient);
	int cmdUser(Client &myClient);
};

#endif