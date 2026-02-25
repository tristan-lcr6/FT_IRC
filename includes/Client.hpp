#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>

class Client
{
  private:
	int _fd;
	std::string _nickName;
	std::string _IP;
	std::string _buff;

  public:
	Client();
	Client(int fd);
	Client(const Client &other);
	Client &operator=(const Client &other);
	~Client();
	int getFd() const;
	int setFd(int fd);
	std::string getIp() const;
	std::string setIp(std::string ip);

	void Client::addBuff(std::string data);
	std::string getBuffer() const;
};

#endif
