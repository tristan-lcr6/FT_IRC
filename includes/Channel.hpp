#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"

class Channel
{
	private:
		std::string _topic;
		std::vector<Client> _clients;
		bool		_i_mode;
		std::vector<Client> _invite_list;
		bool		_t_mode;
		bool		_k_mode;
		std::string	_password;
		std::vector<Client> _operators;
		bool		_l_mode;
		int			_client_limit;
	public:
		Channel(void);
		Channel(const Channel& other);
		Channel &operator=(const Channel &other);
		~Channel();

		bool isInviteOnly(void) const; // returns i_mode
		bool isTopicOpOnly(void) const; // returns t_mode
		bool hasKey(void) const; // returns k_mode
		bool isLimited(void) const; // returns l_mode

		void setInviteOnly(bool b); // sets i_mode to b
		void setTopicOpOnly(bool b); // sets t_mode to b

		void setClientLimit(int limit); // sets l_mode to true and client_limit to limit
		void removeClientLimit(int limit); // sets l_mode to false and client_limit to -1

		void addOperator(Client &cli); // adds cli to operators
		void removeOperator(Client &cli); // removes cli from operators

		void setPassword(Client &cli, std::string psw); // verifies that cli is an operator and sets the password and k_mode to true
		void removePassword(Client &cli); // if cli is operator removes the password and sets k_mode to false

		std::string &getTopic(void) const; // returns topic
		
		void join(Client &cli); // checks if the client can join and adds it to the clients list
		void invite(Client &cli); // adds client to the invite list if i_mode is enabled
};

#endif

