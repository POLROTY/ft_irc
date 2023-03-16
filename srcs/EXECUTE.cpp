/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EXECUTE.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpol <rpol@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 21:52:22 by rpol              #+#    #+#             */
/*   Updated: 2023/03/16 01:04:17 by rpol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void handshake( User *user ) {
	user->printInfo();
    send(user->getFd(), RPL_WELCOME(user).c_str(), RPL_WELCOME(user).length(), MSG_NOSIGNAL);
	send(user->getFd(), RPL_YOURHOST(user).c_str(), RPL_YOURHOST(user).length(), MSG_NOSIGNAL);
	send(user->getFd(), RPL_CREATED(user).c_str(), RPL_CREATED(user).length(), MSG_NOSIGNAL);
	send(user->getFd(), RPL_MYINFO(user).c_str(), RPL_MYINFO(user).length(), MSG_NOSIGNAL);
	send(user->getFd(), NICK(user, user->getNick() ).c_str(), NICK(user, user->getNick() ).length(), MSG_NOSIGNAL);
}

void stream( int client_index, Server & srv ) {
	User * user = srv.user(client_index);

	std::string::size_type pos = user->getBuff().find_first_of('\n');
	if (pos == std::string::npos)
		return;
	
	std::string command = user->getBuff().substr(0, pos);
	user->setBuff( user->getBuff().erase(0, pos + 1) );
	std::istringstream iss(command);
    std::string word;

	if (iss >> word) {
		
		if ( word == "CAP" && !user->isPasswordChecked ) {
			iss >> word;
			if ( word == "LS" )
				user->isIrssi = true;
		} else if ( word == "PASS" ) {
			
			if ( iss >> word ) {
				if ( word == srv.getPassword() ) {
					user->isPasswordChecked = true;
					std::cerr << "A user got password right" << std::endl;
				} else {
					if ( user->isIrssi )
						user->isAlive = false;
					std::string msg = ERR_PASSWDMISMATCH(user);
					send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
				}
			}
		} else if (word == "NICK") {

			if (iss >> word) {
				std::string str;
				if ( nickInUse(word,  srv) ) {
					
					user->setNick(word);
					str = ERR_NICKNAMEINUSE(user);
					send(user->getFd(), str.c_str(), str.length(), MSG_NOSIGNAL);
					user->clearNick();
					
				} else if ( !isValidNickname( word ) ) {
					
					str = ERR_ERRONEUSNICKNAME(user);
					send(user->getFd(), str.c_str(), str.length(), MSG_NOSIGNAL);
					
				} else {

					if ( user->isUserSet ) {
						std::cerr << "SENT NICK MESSAGE" << std::endl;
						str = NICK(user, word);
						std::cerr << str << std::endl;
						send(user->getFd(), str.c_str(), str.length(), MSG_NOSIGNAL);
					}
					user->setNick(word);			
				}
				if ( !user->getNick().empty() && !user->getRealName().empty() && !user->isUserSet ) {
					user->isUserSet = true;
					handshake( user );
				}
			}
		} else if ( word == "USER" ) {
			
			if (iss >> word)
			{
				if (iss >> word)
					user->setName( word );
				if (iss >> word) {
					
					user->setHost( word );
					
					if ( !user->getNick().empty() && !user->isUserSet ) {
						user->isUserSet = true;
						handshake( user );
					}
				}
			}
		} else if (word == "PING") {

			if (iss >> word) {
				std::string str = PONG(user);
				send(user->getFd(), str.c_str(), str.length(), MSG_NOSIGNAL);
				std::cerr << str << std::endl;
			}
		} else if (word == "MODE") {

			if (iss >> word) {
				user->setMode( atoi( word.c_str() ));
				std::cerr << "Changed user mode" << std::endl;
			}
		} else if ( !user->isUserSet ) {
			//maybe send that
			std::cerr << "You need to finish setting up the user before executing commands" << std::endl;
			
		} else if (word == "JOIN") {
			
				iss >> word;
				std::list<Channel*>::iterator it = srv.find_channel( word );
				if (it == srv.channels.end() || srv.channels.empty())
				{	
					srv.channels.push_back(new Channel(word, user));
					std::cerr << "User " << user->getNick() << " created channel " << word << std::endl;
				} else {
					Channel * tmp = *it;
					tmp->join( user );
					std::cerr << "User " << user->getNick() << " was added to channel " << tmp->getName() << std::endl;
				}
		}
		else {

			// std::string str = ":" + user->getName() + " 404 " + user->getNick() + " :" + user->getHost() + " UNKNOWN COMMAND YET\n" ;
			// send( user->getFd(), str.c_str(), str.length(), ERR_NOTIMPLEMENTED );
			send(user->getFd(),  ERR_NOTIMPLEMENTED(word).c_str(), ERR_NOTIMPLEMENTED(word).length(), MSG_NOSIGNAL);
		}
	}

}