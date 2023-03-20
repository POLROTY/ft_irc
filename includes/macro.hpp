#ifndef MACRO_HPP
# define MACRO_HPP

#include "Server.hpp"


// HAND SHAKE
#define RPL_WELCOME(user) (":" + user->getNick() + " 001 " + user->getNick() + " :Welcome to the IRC Network " + user->getNick() + user->getRealName() + "@" + user->getHost() + "\n")
#define RPL_YOURHOST(user) (":" + user->getNick() + " 002 : Your host is " + user->getHost() + ", running version 4.2\n")
#define RPL_CREATED(user) (":" + user->getNick() + " 003 :  This server was created  Tue Feb 14 2023 at 12:34:56 UTC\n")
#define RPL_MYINFO(user) (":" + user->getNick() + " 004 :" + user->getHost() + " 42 iwso ntio\n")


// PING
#define PONG(user) (":" + user->getName() + " PONG " + user->getHost() + "\r\n")

// PASS
#define ERR_PASSWDMISMATCH(user) (":" + user->getName() + " 464 " + user->getNick() + " :" + user->getHost() + " PASSWORD MISSMATCH\n")


// NICK
#define NICK(user, newNick) (":" + user->getNick() + " NICK  " + newNick + "\r\n")
#define ERR_NICKNAMEINUSE(user) (":NICKSERVER 433 * " + user->getNick() + " :Nickname is already in use.\r\n")
#define ERR_ERRONEUSNICKNAME(user) ("432 " + user->getNick() + " :Erroneous nickname\r\n")
#define RPL_WHOISUSER(requester, target) (":" + (requester)->getName() + " 311 " + (requester)->getNick() + " " + (target)->getNick() + " " + (target)->getName() + " " + (target)->getHost() + " * :" + (target)->getRealName() + "\r\n")
#define RPL_WHOISSERVER(requester, target) (":" + (requester)->getName() + " 312 " + (requester)->getNick() + " " + (target)->getNick() + " " + (requester)->getName() + " :This server\r\n")
#define RPL_ENDOFWHOIS(requester, target) (":" + (requester)->getName() + " 318 " + (requester)->getNick() + " " + (target)->getNick() + " :End of /WHOIS list.\r\n")
#define ERR_NOSUCHNICK(requester, nick) (":" + (requester)->getName() + " 401 " + (requester)->getNick() + " " + (nick) + " :No such nick/channel\r\n")                 
#define ERR_NONICKNAMEGIVEN(requester) (":" + (requester)->getName() + " 431 " + (requester)->getNick() + " :No nickname given\r\n")


//MODE
#define ERR_CHANOPRIVSNEEDED(user, channel) (":" + (user)->getName() + " 482 " + (user)->getNick() + " " + (channel) + " :You're not channel operator\r\n")

#define ERR_USERSDONTMATCH(user) (":" + (user)->getName() + " 502 " + (user)->getNick() + " :Cannot change mode for other users\r\n")

// UNKNOWN
#define ERR_NOTIMPLEMENTED(word) (": 449 : " + word + " command not implemented\n")

// CHAN
#define PRIVMSGCHAN(sender, recv, word) (": " + user->getName() + " PRIVMSG " + recv->getNick() + " :" + word + "\r\n")
#define ERR_NOSUCHCHANNEL(user, channel) (":" + user->getName() + " 403 " + user->getNick() + " " + channel + " :No such channel\r\n")
#define ERR_NOSUCHUSERINCHANNEL(user, channel, target) (":" + (user)->getName() + " 441 " + (user)->getNick() + " " + (target) + " " + (channel) + " :No such user in channel\n")
#define RPL_ADDEDCHANOPER(user, channel) (":" + (user)->getName() + " 482 " + (user)->getNick() + " " + (channel) + " :You have been added as a channel operator\n")
#define RPL_REMOVEDCHANOPER(user, channel) (":" + (user)->getName() + " 482 " + (user)->getNick() + " " + (channel) + " :You have been removed as a channel operator\n")

//OPER
#define RPL_YOUREOPER(user) (":" + (user)->getName() + " 381 " + (user)->getNick() + " :You are now an IRC operator\r\n")

#define ERR_NEEDMOREPARAMS(user, command) (":" + (user)->getName() + " 461 " + (user)->getNick() + " " + (command) + " :Not enough parameters\r\n")

#endif