/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PING.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfascia <nathanfascia@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 15:53:05 by nfascia           #+#    #+#             */
/*   Updated: 2023/03/21 15:53:05 by nfascia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	ping_cmd(std::istringstream *iss, std::string word, User *user)
{
	if (*iss >> word) {
		std::string str = PONG(user);
		send(user->getFd(), str.c_str(), str.length(), MSG_NOSIGNAL);
		std::cerr << str << std::endl;
	}
}