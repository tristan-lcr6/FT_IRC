/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jferrand <jferrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 13:26:06 by jferrand          #+#    #+#             */
/*   Updated: 2026/03/06 13:26:08 by jferrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    bool _activ;
    std::vector<std::string> _weatherStr;
    std::vector<std::string> _insultStr;
    std::vector<std::string> _curseWords;
	void init(std::string name);
public:
    Bot();
    Bot(std::string name);
    Bot(const Bot &other);
    Bot &operator=(const Bot &other);
    ~Bot();
    void handleMessage(Client *cli, Channel *channel, std::string message);
    void sendWeather(Channel *channel);
    void sendInsult(Client *cli, Channel *channel);
    bool getActiv();
    void switchActiv();
};

#endif
