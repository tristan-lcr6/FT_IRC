#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <vector>

class Client;
class Channel;

class Bot
{
private:
    std::string _name;
    std::vector<std::string> _weatherStr;
    std::vector<std::string> _insultStr;

public:
    Bot();
    Bot(std::string name);
    Bot(const Bot &other);
    Bot &operator=(const Bot &other);
    ~Bot();
    void handleMessage(Client *cli, Channel *channel, std::string message);
    void sendWeather(Channel *channel);
    void sendInsult(Client *cli, Channel *channel);
};

#endif
