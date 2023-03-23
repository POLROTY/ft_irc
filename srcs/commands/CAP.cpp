/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CAP.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpol <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 15:53:21 by nfascia           #+#    #+#             */
/*   Updated: 2023/03/23 17:35:12 by rpol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	cap_cmd(std::istringstream *iss, std::string word, User *user)
{
	if (*iss >> word) {
		if ( word == "LS" )
			user->isIrssi = true;
	} else {
		std::string msg = ERR_NEEDMOREPARAMS(user, "CAP");
		send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
	}
		
}