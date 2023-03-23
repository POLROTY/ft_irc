/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpol <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 15:53:17 by nfascia           #+#    #+#             */
/*   Updated: 2023/03/23 17:50:44 by rpol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	user_cmd(std::istringstream *iss, std::string *word, User *user)
{
	if (*iss >> *word){
		if (*iss >> *word) {
			user->setName(*word);
		} else {
			std::string msg = ERR_NEEDMOREPARAMS(user, "USER");
			send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
			return;
		}
		if (*iss >> *word) {
			user->setHost(*word);
			if (user->getNick() != "$" && !user->isUserSet) {
				user->isUserSet = true;
				handshake(user);
			}
		} else {
			std::string msg = ERR_NEEDMOREPARAMS(user, "USER");
			send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
		}
	} else {
		std::string msg = ERR_NEEDMOREPARAMS(user, "USER");
		send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
	}
}