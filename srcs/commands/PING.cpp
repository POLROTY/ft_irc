/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PING.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpol <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 15:53:05 by nfascia           #+#    #+#             */
/*   Updated: 2023/03/23 17:41:47 by rpol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	ping_cmd(std::istringstream *iss, std::string word, User *user)
{
	if (*iss >> word) {
		std::string str = PONG(user);
		send(user->getFd(), str.c_str(), str.length(), MSG_NOSIGNAL);
		std::cerr << str << std::endl;
	} else {
		std::string msg = ERR_NEEDMOREPARAMS(user, "PING");
		send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
	}
}