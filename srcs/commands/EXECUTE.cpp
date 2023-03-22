/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EXECUTE.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hspriet <hspriet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 21:52:22 by rpol              #+#    #+#             */
/*   Updated: 2023/03/22 15:45:29 by hspriet          ###   ########.fr       */
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
		} else if (word == "QUIT") {
			user->isAlive = false;
		} else if (word == "MODE") {
			mode_cmd(&iss, user, srv);
		} else if ( !user->isUserSet ) {
			//maybe send that
			std::cerr << "You need to finish setting up the user before executing commands" << std::endl;
		} else if (word == "WHO") {
            who_cmd(&iss, &word, user, srv);
		} else if (word == "OPER") {
    		oper_cmd(&iss, user, srv);      
		} else if (word == "kill") {
			kill_cmd(&iss, user, srv);
		}  else if (word == "INVITE") {
			invite_cmd(&iss, word, user, srv);
		} else if (word == "KICK") {
			kick_cmd(&iss, user, srv);
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
		} else if (word == "NOTICE") {
			notice_cmd(&iss, word, user, srv);
		} else {
			send(user->getFd(),  ERR_NOTIMPLEMENTED(word).c_str(), ERR_NOTIMPLEMENTED(word).length(), MSG_NOSIGNAL);
		}
	}
}