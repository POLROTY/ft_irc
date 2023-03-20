/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EXECUTE.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpol <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 21:52:22 by rpol              #+#    #+#             */
/*   Updated: 2023/03/20 18:39:31 by rpol             ###   ########.fr       */
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
					user->isPasswordChecked = true;
					std::cerr << "A user got password right" << std::endl;
				} else {
					if ( user->isIrssi )
						user->isAlive = false;
					std::string msg = ERR_PASSWDMISMATCH(user);
					send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
				}
			}
		} else if ( !user->isPasswordChecked ) {
			
			user->isAlive = false;
			std::string msg = ERR_PASSWDMISMATCH(user);
			send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
			
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

			std::string target, mode_changes, target_user_nick;
            if (iss >> target >> mode_changes) {
                if (target[0] == '#') {  // Channel mode
                    std::list<Channel*>::iterator it = srv.find_channel(target);
                    if (it != srv.channels.end()) {
                        Channel *channel = *it;
                        if (channel->is_operator(user)) {
							if ( iss >> target_user_nick ) {
								User * target_user = channel->find_user_by_nickname(target_user_nick);
								if (target_user) {
									channel->update_modes(mode_changes, target_user);
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
            if (iss >> word) {
                if (word[0] == '#') {
                    std::list<Channel*>::iterator it = srv.find_channel(word);
                    if (it != srv.channels.end()) {
                        std::string msg = (*it)->who(user);
                        send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
                    } else {
                        std::string msg = ERR_NOSUCHCHANNEL(user, word);
                        send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
                    }
            	}
			}
		} else if (word == "OPER") {
             std::string username, password;
            if (iss >> username >> password) {
                if (srv.is_valid_oper(username, password)) {
                    user->isServerOperator = true;
                    std::string msg = RPL_YOUREOPER(user);
                    send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
                } else {
                    std::string msg = ERR_PASSWDMISMATCH(user);
                    send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
                }
            } else {
                std::string msg = ERR_NEEDMOREPARAMS(user, "OPER");
                send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
            }
		} else if (word == "WHOIS") {
            if (iss >> word) {
                User* targetUser = srv.find_user_by_nickname(word);
                if (targetUser) {
                    std::string msg = RPL_WHOISUSER(user, targetUser) + RPL_WHOISSERVER(user, targetUser) + RPL_ENDOFWHOIS(user, targetUser);
                    send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
                } else {
                    std::string msg = ERR_NOSUCHNICK(user, word);
                    send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
                }
            } else {
                std::string msg = ERR_NONICKNAMEGIVEN(user);
                send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
            }
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
					std::string current_topic = (*it)->get_topic();
					if (!current_topic.empty()) {
						std::string msg = RPL_TOPIC(user, word, current_topic);
						send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
					}
					std::cerr << "User " << user->getNick() << " was added to channel " << tmp->getName() << std::endl;
				}
		} else if (word == "TOPIC") {
			if (iss >> word) { // Extract the channel name
				std::list<Channel*>::iterator it = srv.find_channel(word);
				if (it != srv.channels.end()) {
					std::string new_topic;
					if (getline(iss, new_topic, ':')) { // If a new topic is specified
						getline(iss, new_topic); // Read the rest of the new topic
						// Remove any trailing newline characters from the new topic
						new_topic.erase(std::remove(new_topic.begin(), new_topic.end(), '\n'), new_topic.end());
						new_topic.erase(std::remove(new_topic.begin(), new_topic.end(), '\r'), new_topic.end());

						// Set the new topic for the channel
						(*it)->set_topic(new_topic);
						(*it)->broadcast_new_topic();
					} else {
						// If no new topic is specified, return the current topic
						std::string current_topic = (*it)->get_topic();
						std::string msg = RPL_TOPIC(user, word, current_topic);
						send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
					}
				} else {
					std::string msg = ERR_NOSUCHCHANNEL(user, word);
					send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
				}
			}
        } else if (word == "PRIVMSG") {
				iss >> word; // Extract the target (channel or user nickname)

				// Check if the target is a channel (starts with a '#' character)
				if (word[0] == '#') {
					std::list<Channel*>::iterator it = srv.find_channel(word);
					if (it != srv.channels.end()) {
						// Broadcast the message to the channel
						getline(iss, word, ':'); // Skip the colon before the message content
						std::string message;
						getline(iss, message); // Read the rest of the message

						// Remove any trailing newline characters from the message
						message.erase(std::remove(message.begin(), message.end(), '\n'), message.end());
						message.erase(std::remove(message.begin(), message.end(), '\r'), message.end());

						(*it)->broadcast(word + message, user);
					} else {
						// Handle the case where the channel was not found
						// (e.g., send an error message back to the user)
					}
				} else {
					// The target is a user nickname
					std::string receiver_nickname = word;
					getline(iss, word, ':'); // Skip the colon before the message content
					std::string message;
					getline(iss, message); // Read the rest of the message

					// Remove any trailing newline characters from the message
					message.erase(std::remove(message.begin(), message.end(), '\n'), message.end());
					message.erase(std::remove(message.begin(), message.end(), '\r'), message.end());

					// Send the private message to the target user
					srv.send_private_message(user->getNick(), receiver_nickname, message);
				}
		} else {

			// std::string str = ":" + user->getName() + " 404 " + user->getNick() + " :" + user->getHost() + " UNKNOWN COMMAND YET\n" ;
			// send( user->getFd(), str.c_str(), str.length(), ERR_NOTIMPLEMENTED );
			send(user->getFd(),  ERR_NOTIMPLEMENTED(word).c_str(), ERR_NOTIMPLEMENTED(word).length(), MSG_NOSIGNAL);
		}
	}
}