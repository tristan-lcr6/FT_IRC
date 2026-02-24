#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>

class Client
{
  private:
	int _fd;
	std::string _nickname;
	std::string _buff;

  public:
	Client(void);
	Client(const Client &other);
	Client &operator=(const Client &other);
	~Client();
};

#endif
