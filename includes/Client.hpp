#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <vector>
#include <sys/socket.h>


class Client
{
  private:
	int _fd;
	int _authStatus;
	std::string _nickName;
	std::string _userName;
	std::string _realName;
	std::string _IP;
	std::string _buff;

  public:
  	// ************************************************************************** //
	// Constructor / Destructor
	// ************************************************************************** //
	Client();
	Client(int fd);
	Client(const Client &other);
	Client &operator=(const Client &other);
	~Client();

	// ************************************************************************** //
	// Setters
	// ************************************************************************** //

	int setFd(int fd);
	std::string setIp(std::string ip);
	void setGrade(int i);
	void setNickName(std::string name);
	void setRealName(std::string name);
	void setUserName(std::string name);
	std::string setBuffer(std::string str);

	// ************************************************************************** //
	// Getters
	// ************************************************************************** //

	const std::string &getNickname(void) const;
	int getAuthStatus() const;
	std::string getIp() const;
	std::string getNickName() const;
	std::string getUserName() const;
	std::string getBuffer() const;
	std::string getRealName() const;
	int getFd() const;
	
	// ************************************************************************** //
	// Buffer management
	// ************************************************************************** //

	void addBuff(std::string data);
	// void eraseBuffer(size_t start, size_t size);
	void clearBuffer();
	
	// ************************************************************************** //
	// Message management
	// ************************************************************************** //

	void sendMessageOnClientFd(std::string& msg) const;
	
	bool operator!=(const Client &other);
	bool operator==(const Client &c);

	class ClientException : public std::exception
	{
		private : 
		 	std::string _mess;
		public :
		ClientException(std::string mess) : _mess(mess){}
		virtual ~ClientException() throw() {}
        virtual const char* what() const throw() 
        {
            return _mess.c_str();
        }
	};

};

std::ostream &operator<<(std::ostream &os, const Client &c);

std::ostream& operator<<(std::ostream& dataStream, const std::vector<std::string>& vector);

#endif
