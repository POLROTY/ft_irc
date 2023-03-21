/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfascia <nathanfascia@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 15:53:17 by nfascia           #+#    #+#             */
/*   Updated: 2023/03/21 15:53:17 by nfascia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	user_cmd(std::istringstream *iss, std::string *word, User *user)
{
	if (*iss >> *word){
		if (*iss >> *word)
			user->setName(*word);
		if (*iss >> *word) {
			user->setHost(*word);
			if (user->getNick() != "$" && !user->isUserSet) {
				user->isUserSet = true;
				handshake(user);
			}
		}
	}
}