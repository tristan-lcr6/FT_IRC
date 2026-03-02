#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <vector>
#include <cstdlib>
#include <limits>

class Channel
{
private:
	std::string _name;
	std::string _topic;
	std::vector<Client *> _clients;
	bool _i_mode;
	std::vector<Client *> _invite_list;
	bool _t_mode;
	bool _k_mode;
	std::string _password;
	std::vector<Client *> _operators;
	bool _l_mode;
	size_t _client_limit;

public:

	// ************************************************************************** //
	// Constructors / Destructor
	// ************************************************************************** //
	Channel(void);
	Channel(Client &cli, std::string name); // constructor that puts cli as the first operator
	Channel(const Channel &other);
	Channel &operator=(const Channel &other);
	~Channel();

	// ************************************************************************** //
	// Getters
	// ************************************************************************** //
	bool isInviteOnly(void) const;	// returns i_mode
	bool isTopicOpOnly(void) const; // returns t_mode
	bool hasKey(void) const;		// returns k_mode
	bool isLimited(void) const;		// returns l_mode
	const std::string &getName(void) const;	 // returns name
	const std::string &getTopic(void) const; // returns topic

	// ************************************************************************** //
	// Client management
	// ************************************************************************** //

	Client *getClient(std::string nick);	 // returns the client corresponding to the nickname
	void join(Client &cli);								 // checks if the client can join and adds it to the clients list
	void join(Client &cli, std::string pwd);			 // checks if the client can join and the password is right and adds it to the clients list
	void kick(Client &cli);
	

	// ************************************************************************** //
	// Operator management
	// ************************************************************************** //

	void addOperator(Client *cli); // adds cli to operators
	void addOperator(std::string nick); // adds the client with the nickname nick to operators
	void removeOperator(Client *cli); // removes cli from operators
	void removeOperator(std::string nick); // removes the client with the nickname nick from operators

	// ************************************************************************** //
	// Invite / Topic management
	// ************************************************************************** //

	void setInviteOnly(bool b);	 // sets i_mode to b
	void invite(Client &cli);	// adds client to the invite list if i_mode is enabled
	void setTopicOpOnly(bool b); // sets t_mode to b
	void setTopic(std::string topic); // sets the topic

	// ************************************************************************** //
	// Mode management
	// ************************************************************************** //

	static bool modeWithParam(char c, bool add);
	void applyMode(char c, bool add);					 // applies the mode assigned to the char c
	void applyMode(char c, bool add, std::string param); // applies the mode assigned to the char c with the params

	// ************************************************************************** //
	// Password / Limit
	// ************************************************************************** //

	void setPassword(std::string psw); // sets the password and k_mode to true
	void removePassword(void);		   // removes the password and sets k_mode to false
	void setClientLimit(size_t limit); // sets l_mode to true and client_limit to limit
	void setClientLimit(std::string limit_str); // sets l_mode to true and client_limit to limit
	void removeClientLimit();					// sets l_mode to false and client_limit to -1

	// ************************************************************************** //
	// Messaging
	// ************************************************************************** //

	void sendChannelMessage(Client &myClient, std::string message);//sen message to all client in named channel


	friend std::ostream &operator<<(std::ostream &os, const Channel &channel);
};

#include <iostream>

#endif
