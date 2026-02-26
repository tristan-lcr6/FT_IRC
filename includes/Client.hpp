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
	Client();
	Client(int fd);
	Client(const Client &other);
	Client &operator=(const Client &other);
	~Client();
	int setFd(int fd);
	std::string setIp(std::string ip);
	const std::string &getNickname(void) const;
	void addBuff(std::string data);
	std::string setBuffer(std::string str);
	// void eraseBuffer(size_t start, size_t size);
	void clearBuffer();
	bool operator==(const Client &c);
	void setGrade(int i);
	void setNickName(std::string name);
	void setRealName(std::string name);
	void setUserName(std::string name);
	int getAuthStatus() const;
	std::string getIp() const;
	std::string getNickName() const;
	std::string getUserName() const;
	std::string getBuffer() const;
	std::string getRealName() const;
	int getFd() const;
	bool operator!=(const Client &other);
	void sendMessage(std::string& msg) const;

};

std::ostream &operator<<(std::ostream &os, const Client &c);

std::ostream& operator<<(std::ostream& dataStream, const std::vector<std::string>& vector);

#endif
