#ifndef MACRO_HPP
# define MACRO_HPP

#include "Server.hpp"



#define RPL_WELCOME(user) (":" + user->getName() + " 001 " + user->getNick() + " :Welcome to the Internet Relay Network " + user->getName() + "\n")

#define RPL_YOURHOST(user) (":" + user->getName() + " 002 " + user->getNick() + " :Your host is " + user->getHost() + ", running version 42\n")

#define RPL_CREATED(user) (":" + user->getName() + " 003 " + user->getNick() + " :This server was created at saint Gliglin\n")

#define RPL_MYINFO(user) (":" + user->getName() + " 002 " + user->getNick() + " :" + user->getHost() + " 42 iwso ntio\n")

#define NICK(user, nick) (":" + user->getName() + " NICK " + nick + "\n")

#define PONG(user) (":" + user->getName() + " PONG " + user->getHost() + "\n")

#define ERR_PASSWDMISMATCH(user) (":" + user->getName() + " 464 " + user->getNick() + " :" + user->getHost() + " PASSWORD MISSMATCH\n")


#endif