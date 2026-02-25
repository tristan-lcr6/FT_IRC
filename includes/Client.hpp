#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>

class Client
{
  private:
	int _fd;
	std::string _nickname;
	std::string _IP;
	std::string _buff;

  public:
	Client();
	Client(int fd);
	Client(const Client &other);
	Client &operator=(const Client &other);
	~Client();
	int GetFd() const;
	int SetFd(int fd);
	std::string GetIp() const;
	std::string SetIp(std::string ip);

};

#endif
