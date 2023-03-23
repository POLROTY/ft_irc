/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpol <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 21:46:24 by rpol              #+#    #+#             */
/*   Updated: 2023/03/23 17:38:34 by rpol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

bool isValidNickname(const std::string& nickname) {
    // Check if the nickname is too long (max length is 9 characters)
    if (nickname.length() > 9) {
        return false;
    }
    
    // Check if the nickname starts with a letter or underscore
    if (!std::isalpha(nickname[0]) && nickname[0] != '_') {
        return false;
    }
    
    // Check if the remaining characters are alphanumeric or underscore
    for (size_t i = 1; i < nickname.length(); i++) {
        if (!std::isalnum(nickname[i]) && nickname[i] != '_') {
            return false;
        }
    }
    
    // The nickname is valid
    return true;
}

// Check if a given nickname is already in use on the given server.
// Returns true if the nickname is in use, false otherwise.
bool nickInUse(std::string nickToCheck, Server& srv) {
    // Iterate over the list of users on the server
    std::list<User*>::iterator it = srv.getUsersBegin();
    std::list<User*>::iterator ite = srv.getUsersEnd();
    while (it != ite) {
        // If the nickname is already in use, return true
        if (nickToCheck == (*it)->getNick() && (*it)->isAlive && nickToCheck != "$") {
            return true;
        }
        it++;
    }
    // The nickname is not in use
    return false;
}

void	nick_cmd(std::istringstream *iss, std::string *word, User *user, Server &srv)
{
	if (*iss >> *word) {
		std::string str;
		if ( nickInUse(*word,  srv)) {
			str = ERR_NICKNAMEINUSE(user, *word);
			send(user->getFd(), str.c_str(), str.length(), MSG_NOSIGNAL);
		} else if (!isValidNickname(*word)) {
			str = ERR_ERRONEUSNICKNAME(user);
			send(user->getFd(), str.c_str(), str.length(), MSG_NOSIGNAL);
		} else {
			if (user->isUserSet) {
				std::cerr << "SENT NICK MESSAGE" << std::endl;
				str = NICK(user, *word);
				std::cerr << str << std::endl;
				send(user->getFd(), str.c_str(), str.length(), MSG_NOSIGNAL);
			}
			user->setNick(*word);			
		}
		if (user->getNick() != "$" && user->getRealName() != "$" && !user->isUserSet) {
			user->isUserSet = true;
			handshake(user);
		}
	} else {
		std::string msg = ERR_NEEDMOREPARAMS(user, "NICK");
		send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
	}
}