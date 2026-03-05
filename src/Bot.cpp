#include "Bot.hpp"
#include "Channel.hpp"

Bot::Bot(std::string name) : _name(name)
{
    _weatherStr.push_back("Weather will be a great upon 42 paris tommorow.");
    _weatherStr.push_back("Weather will be really shitty.");
    _weatherStr.push_back("It will be raining bombs: Once again Donald trump want to destroy the world.");
    _insultStr.push_back("is the reason why instructions are written on shampoo bottles.");
    _insultStr.push_back("look likes he’d try to keep a fish from drowning.");
    _insultStr.push_back("You have your entire life to be a jerk. Why not take today off?");
    _insultStr.push_back("Somewhere, a tree is working very hard to provide you with oxygen. Go apologize to it.");

    
}

Bot::Bot()
{
    _name = "bot";
    _weatherStr.push_back("Weather will be a great upon 42 paris tommorow.");
    _weatherStr.push_back("Weather will be really shitty.");
    _weatherStr.push_back("It will be raining bombs: Once again Donald trump want to destroy the world.");
}

Bot::Bot(const Bot &other)
{
    if (this != &other)
    {
        *this = other;
    }
    return;
}

Bot &Bot::operator=(const Bot &other)
{

    this->_weatherStr = other._weatherStr;
    this->_name = other._name;
    return (*this);
}

Bot::~Bot(void)
{
    _weatherStr.clear();
    return;
}

void Bot::handleMessage(Client *cli, Channel *channel, std::string message)
{
    for (size_t i = 0; i < message.size(); i++)
    {
        message[i] = tolower(message[i]);
    }
    if (message.find("weather") != std::string::npos)
        sendWeather(channel);
    if (message.find("fuck") != std::string::npos || message.find("asshole") != std::string::npos || message.find("bitch") != std::string::npos)
        sendInsult(cli, channel);
}

void Bot::sendWeather(Channel *channel)
{
    std::string message = _name + "!" + "Bot42-fr" + "@" + "127.0.0.1";
    size_t index = rand() % 3;
    message += " " + _weatherStr[index];
    channel->sendChannelMessageBot(message);
}

void Bot::sendInsult(Client *cli, Channel *channel)
{
    std::string message = _name + "!" + "Bot42-fr" + "@" + "127.0.0.1";
    size_t index = rand() % 4;
    message += " " + _insultStr[index];
    channel->sendChannelMessageBot(message);
}