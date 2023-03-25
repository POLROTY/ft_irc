#ifndef MACRO_HPP
# define MACRO_HPP

#include "Server.hpp"


// HAND SHAKE
#define RPL_WELCOME(user) (":" + user->getHost() + " 001 " + user->getNick() + " Welcome to the IRC Network " + user->getName() + "\n")
#define RPL_YOURHOST(user) (":" + user->getHost() + " 002 " + user->getNick() + " Your host is " + Server::instance->getHostName() + ", running version 4.2\n")
#define RPL_CREATED(user) (":" + user->getHost() + " 003 " + user->getNick() + " This server was created " + Server::instance->getStartDate() + "\n")
#define RPL_MYINFO(user) (":" + user->getHost() + " 004 " + user->getNick() + " " + Server::instance->getHostName() + " v4.2 io iob\n")


// PING
#define PONG(user) (":" + user->getHost() + " PONG :" + user->getNick() + "\r\n")

// PASS
#define ERR_PASSWDMISMATCH(user) (":" + user->getName() + " 464 " + user->getNick() + " :" + user->getHost() + " PASSWORD MISSMATCH\n")


// NICK
#define NICK(user, newNick) (":" + user->getName() + " NICK " + newNick + "\r\n")
#define ERR_NICKNAMEINUSE(user, nick) (":" + user->getHost() + " 433 " + user->getNick() + " " + nick + " :Nickname is already in use.\r\n")
#define ERR_ERRONEUSNICKNAME(user) (":" + user->getHost() + " 432 " + user->getNick() + " :Erroneous nickname\r\n")
#define RPL_WHOISUSER(requester, target) (":" + (requester)->getName() + " 311 " + (requester)->getNick() + " " + (target)->getNick() + " " + (target)->getName() + " " + (target)->getHost() + " * :" + (target)->getRealName() + "\r\n")
#define RPL_WHOISSERVER(requester, target) (":" + (requester)->getName() + " 312 " + (requester)->getNick() + " " + (target)->getNick() + " " + (requester)->getName() + " :This server\r\n")
#define RPL_ENDOFWHOIS(requester, target) (":" + (requester)->getName() + " 318 " + (requester)->getNick() + " " + (target)->getNick() + " :End of /WHOIS list.\r\n")
#define ERR_NOSUCHNICK(requester, nick) (":" + (requester)->getName() + " 401 " + (requester)->getNick() + " " + (nick) + " :No such nick/channel\r\n")                 
#define ERR_NONICKNAMEGIVEN(requester) (":" + (requester)->getName() + " 431 " + (requester)->getNick() + " :No nickname given\r\n")


//MODE
#define ERR_CHANOPRIVSNEEDED(user, channel) (":" + (user)->getName() + " 482 " + (user)->getNick() + " " + (channel) + " :You're not channel operator\r\n")

#define ERR_USERSDONTMATCH(user) (":" + (user)->getName() + " 502 " + (user)->getNick() + " :Cannot change mode for other users\r\n")

// UNKNOWN
#define ERR_NOTIMPLEMENTED(word) (":" + user->getHost() + " 449 : " + word + " command not implemented\n")

// CHAN
#define PRIVMSGCHAN(sender, recv, word) (": " + user->getName() + " PRIVMSG " + recv->getNick() + " :" + word + "\r\n")
#define ERR_NOSUCHCHANNEL(user, channel) (":" + user->getName() + " 403 " + user->getNick() + " " + channel + " :No such channel\r\n")
#define ERR_NOSUCHUSERINCHANNEL(user, channel, target) (":" + (user)->getName() + " 441 " + (user)->getNick() + " " + (target) + " " + (channel) + " :No such user in channel\n")
#define ERR_USERONCHANNEL(user, nick, channel) ( ":" + user->getHost() + " 443 " + user->getNick() + " " + nick + " " + channel + " :is already on channel\n" )
#define ERR_NOTONCHANNEL(user, channel) ( ":" + user->getHost() + " 442 " + user->getNick() + " " + channel + " :You're not on that channel" + "\n" )
#define RPL_ADDEDCHANOPER(user, channel, operator) (":" + (user)->getName() + " 482 " + (user)->getNick() + " " + (channel) + " :You have been added as a channel operator by operator : "+ operator->getNick() + " \n")
#define RPL_REMOVEDCHANOPER(user, channel, operator) (":" + (user)->getName() + " 482 " + (user)->getNick() + " " + (channel) + " :You have been removed as a channel operator by operator : "+ operator->getNick() + " \n")
#define ERR_BANNEDFROMCHAN(user, channel) ( ":" + user->getHost() + " 474 " + user->getNick() + " " + channel->getName() + " :Cannot join " + channel->getName() + " (+b) - you are banned\n" )
#define RPL_UNBANUSER(user, channel, operator) (":" + user->getName() + " MODE " + channel->getName() + " -b " + user->getNick() + " : You have been unbanned from " + channel->getName() + " by operator : "+ operator->getNick() + " \r\n")
#define ERR_NEEDINVITE(user, channel) ( ":" + user->getHost() + " 473 " + user->getNick() + " " + channel->getName() + " :Cannot join " + channel->getName() + " (+i) - you must be invited\n" )

//TOPIC
#define RPL_TOPIC(user, channel, topic) (":" + (user)->getName() + " 332 " + (user)->getNick() + " " + (channel) + " :" + (topic) + "\n")

//OPER
#define RPL_YOUREOPER(user) (":" + (user)->getName() + " 381 " + (user)->getNick() + " :You are now an IRC operator\r\n")
#define ERR_NOPRIVILEGES(user) ( ":" + user->getHost() + " 481 " + user->getNick() + " :Permission Denied- You're not an IRC operator\r\n" )
#define ERR_YOUDEAD(victim, killer) ( ":" + victim->getHost() + " 483 " + victim->getNick() + " :You have been killed from server by the IRC operator : " + killer->getNick() + "\r\n" )
#define ERR_SRVDEAD(victim, killer) ( ":" + victim->getHost() + " 483 " + victim->getNick() + " :Server has been killed by the IRC server operator : " + killer->getNick() + "\r\n" )

#define ERR_NEEDMOREPARAMS(user, command) (":" + (user)->getName() + " 461 " + (user)->getNick() + " " + (command) + " :Not enough parameters\r\n")

#endif
