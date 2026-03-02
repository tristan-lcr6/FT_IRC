/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlecuyer <tlecuyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:22:56 by jferrand          #+#    #+#             */
/*   Updated: 2026/03/02 17:08:12 by tlecuyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Channel.hpp"
#include "utils.hpp"
#include "Client.hpp"
#include <arpa/inet.h> //-> for inet_ntoa()
#include <csignal>     //-> for signal()
#include <cstdio>      //-> for signal()
#include <cstdlib>
#include <cstring>
#include <fcntl.h> //-> for fcntl()
#include <iostream>
#include <netinet/in.h> //-> for sockaddr_in
#include <poll.h>       //-> for poll()
#include <sys/socket.h> //-> for socket()
#include <sys/types.h>  //-> for socket()
#include <unistd.h>     //-> for close()
#include <vector>       //-> for vector

#define BLACK "\033[90m"
#define RED "\033[91m"
#define GREEN "\033[92m"
#define YELLOW "\033[93m"
#define BLUE "\033[94m"
#define PURPLE "\033[95m"
#define CYAN "\033[96m"
#define END "\033[0m"

class Server
{
private:
    int _port;
    int _serSocketFd;
    static bool _signal;
    std::string _password;

    std::vector<Client> _clients;
    std::vector<struct pollfd> _fds;
    std::vector<Channel *> _channels;

public:

    // ************************************************************************** //
    //  Server Constructor Destructor
    // ************************************************************************** //
    Server();
    Server(const Server &other);
    Server &operator=(const Server &other);
    ~Server();

    // ************************************************************************** //
    //  Core Server Logic
    // ************************************************************************** //
    void serverInit();
    void serSocket();
    void acceptNewClient();
    void receiveNewData(int fd);
    void closeFds();
    void clearClient(int fd);
    static void signalHandler(int signum);

    
    // ************************************************************************** //
    // --- Data Management (Server.cpp) ---
    // ************************************************************************** //
    void setPort(int port);
    void setPassword(std::string pswd);
    Client &findClientByFd(int fd);
    int findFdByNickName(std::string nickName);
    Channel &getChannel(std::string name);
    bool isAlreadyChannel(Channel *&channel, const std::string &channel_name);

    
    // ************************************************************************** //
    // --- Command Execution (Execution.cpp) ---
    // ************************************************************************** //
    void execute(Client &cli);

    
    // ************************************************************************** //
    // --- IRC Commands (Dossier commands/) ---
    // ************************************************************************** //
    void capLs(Client &cli);
    void cmdPass(Client &cli);
    void cmdNick(Client &cli);
    void cmdUser(Client &cli);

    
    // ************************************************************************** //
    // Channel.cpp
    // ************************************************************************** //
    void cmdJoin(Client &cli);
    void JoinMessage(std::string channelName, Client &cli);
    void cmdMode(Client &cli);
	void broadcastNick(Client &cli, std::string &nick);
    void cmdKick(Client &cli);
    void cmdInvite(Client &cli);
    void cmdTopic(Client &cli);
    void cmdPrivMsg(Client &cli);
    
    //! --- to RM  ---
    void cmdTest(Client &myClient);

    
    // ************************************************************************** //
    // --- Exceptions ---
    // ************************************************************************** //
    class ServerException : public std::exception
    {
    private:
        std::string _mess;

    public:
        ServerException(std::string mess) : _mess(mess) {}
        virtual ~ServerException() throw() {}
        virtual const char *what() const throw() { return _mess.c_str(); }
    };
};


// ************************************************************************** /
// --- Global Utils (utils.cpp) ---
// ************************************************************************** //
bool isValidString(const std::string &str);
std::vector<std::string> split(std::string s, std::string delimiter);

#endif
