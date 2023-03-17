#ifndef MACRO_HPP
# define MACRO_HPP

#include "Server.hpp"


// HAND SHAKE
#define RPL_WELCOME(user) (":" + user->getName() + " 001 " + user->getNick() + " :Welcome to the Internet Relay Network " + user->getName() + "\n")
#define RPL_YOURHOST(user) (":" + user->getName() + " 002 :Your host is " + user->getHost() + ", running version 42\n")
#define RPL_CREATED(user) (":" + user->getName() + " 003 :This server was created at saint Gliglin\n")
#define RPL_MYINFO(user) (":" + user->getName() + " 004 :" + user->getHost() + " 42 iwso ntio\n")


// PING
#define PONG(user) (":" + user->getName() + " PONG " + user->getHost() + "\r\n")

// PASS
#define ERR_PASSWDMISMATCH(user) (":" + user->getName() + " 464 " + user->getNick() + " :" + user->getHost() + " PASSWORD MISSMATCH\n")


// NICK
#define NICK(user, newNick) (":" + user->getNick() + " NICK  " + newNick + "\r\n")
#define ERR_NICKNAMEINUSE(user) (":NICKSERVER 433 * " + user->getNick() + " :Nickname is already in use.\r\n")
#define ERR_ERRONEUSNICKNAME(user) ("432 " + user->getNick() + " :Erroneous nickname\r\n")

// UNKNOWN
#define ERR_NOTIMPLEMENTED(word) (": 449 : " + word + " command not implemented\n")

// PRIVMSG CHAN
#define PRIVMSGCHAN(sender, recv, word) (": " + user->getName() + " PRIVMSG " + recv->getNick() + " :" + word + "\r\n")


#endif