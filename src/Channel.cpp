#include "Channel.hpp"

// Default constructor
Channel::Channel(void)
{
    // std::cout << "Default constructor called" << std::endl;
    return ;
}

// Copy constructor
Channel::Channel(const Channel &other)
{
    // std::cout << "Copy constructor called" << std::endl;
    (void) other;
    return ;
}

// Assignment operator overload
Channel &Channel::operator=(const Channel &other)
{
    // std::cout << "Assignment operator called" << std::endl;
    (void) other;
    return (*this);
}

// Destructor
Channel::~Channel(void)
{
    // std::cout << "Destructor called" << std::endl;
    return ;
}

