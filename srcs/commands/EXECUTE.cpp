/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EXECUTE.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfascia <nathanfascia@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 21:52:22 by rpol              #+#    #+#             */
/*   Updated: 2023/03/21 16:46:53 by nfascia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void handshake(User *user) {

	user->printInfo();
    send(user->getFd(), RPL_WELCOME(user).c_str(), RPL_WELCOME(user).length(), MSG_NOSIGNAL);
	send(user->getFd(), RPL_YOURHOST(user).c_str(), RPL_YOURHOST(user).length(), MSG_NOSIGNAL);
	send(user->getFd(), RPL_CREATED(user).c_str(), RPL_CREATED(user).length(), MSG_NOSIGNAL);
	send(user->getFd(), RPL_MYINFO(user).c_str(), RPL_MYINFO(user).length(), MSG_NOSIGNAL);
	send(user->getFd(), NICK(user, user->getNick() ).c_str(), NICK(user, user->getNick() ).length(), MSG_NOSIGNAL);

}

void stream( int client_index, Server & srv ) {
	User * user = srv.get_user_by_index(client_index);

	std::string::size_type pos = user->getBuff().find_first_of('\n');
	if (pos == std::string::npos)
		return;
	
	std::string command = user->getBuff().substr(0, pos);
	user->setBuff( user->getBuff().erase(0, pos + 1) );
	std::istringstream iss(command);
    std::string word;

	if (iss >> word) {
		
		if ( word == "CAP" && !user->isPasswordChecked ) {
			cap_cmd(&iss, word);
			if ( word == "LS" )
				ls_cmd(user);
		} else if ( word == "PASS" ) {
			pass_cmd(&iss, &word, user, srv);
		} else if ( !user->isPasswordChecked ) {
			
			user->isAlive = false;
			std::string msg = ERR_PASSWDMISMATCH(user);
			send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
			
		} else if (word == "NICK") {
			nick_cmd(&iss, &word, user, srv);
		} else if ( word == "USER" ) {
			user_cmd(&iss, &word, user);
		} else if (word == "PING") {
			ping_cmd(&iss, word, user);
		} else if (word == "MODE") {

			std::string target, mode_changes, target_user_nick;
            if (iss >> target >> mode_changes) {
                if (target[0] == '#') {  // Channel mode
                    std::list<Channel*>::iterator it = srv.find_channel(target);
                    if (it != srv.getChannelsEnd()) {
                        Channel *channel = *it;
                        if (channel->is_operator(user) || user->isServerOperator) {
							if ( iss >> target_user_nick ) {
								User * target_user = channel->find_user_by_nickname(target_user_nick);
								if (target_user) {
									std::cerr << "Found target " << target_user_nick << std::endl;
									channel->update_modes(mode_changes, target_user);
									if (channel->is_operator_empty())
										srv.remove_channel(channel);
								} else {
									std::string msg = ERR_NOSUCHUSERINCHANNEL(user,target,target_user_nick);
                      			  	send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
								}
							} else {
                            	channel->update_modes(mode_changes, user);
							}
                        } else {
                            std::string msg = ERR_CHANOPRIVSNEEDED(user, target);
                            send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
                        }
                    } else {
                        std::string msg = ERR_NOSUCHCHANNEL(user, target);
                        send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
                    }
                } else {  // User mode
                    if (user->getNick() == target) {
                        user->update_modes(mode_changes);
                    } else {
                        std::string msg = ERR_USERSDONTMATCH(user);
                        send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
                    }
                }
            } else {
                std::string msg = ERR_NEEDMOREPARAMS(user, "MODE");
                send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
            }
		} else if ( !user->isUserSet ) {
			//maybe send that
			std::cerr << "You need to finish setting up the user before executing commands" << std::endl;
			
		} else if (word == "WHO") {
            who_cmd(&iss, &word, user, srv);
		} else if (word == "OPER") {
            oper_cmd(&iss, user, srv);
		} else if (word == "WHOIS") {
			whois_cmd(&iss, word, user, srv);
        } else if (word == "JOIN") {
			join_cmd(&iss, word, user, srv);
		} else if (word == "PART") {
			part_cmd(&iss, word, user, srv);
		} else if (word == "TOPIC") {
			topic_cmd(&iss, word, user, srv);
        } else if (word == "PRIVMSG") {
			privmsg_cmd(&iss, word, user, srv);
		} else {
			// std::string str = ":" + user->getName() + " 404 " + user->getNick() + " :" + user->getHost() + " UNKNOWN COMMAND YET\n" ;
			// send( user->getFd(), str.c_str(), str.length(), ERR_NOTIMPLEMENTED );
			send(user->getFd(),  ERR_NOTIMPLEMENTED(word).c_str(), ERR_NOTIMPLEMENTED(word).length(), MSG_NOSIGNAL);
		}
	}
}