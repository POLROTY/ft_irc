#include "Channel.hpp"
#include "irc.hpp"
#include <algorithm>
#include <sys/socket.h>


////////// methods //////////
Channel::Channel(void) {
	return;
}

Channel::Channel( const std::string & name, User * user) : name(name) {
	users.push_back( user );
	operators.push_back( user ); // Add the first user as an operAtor
	std::string info = ":" + user->getName() + " JOIN " + name + "\r\n";
    broadcast_info(info);
	this->isInviteOnly = false;;
}

Channel::Channel(const Channel &other) {
	*this = other;
	return;
}

Channel &Channel::operator=(const Channel &other) {
	if (this != &other) {
		name = other.name;
		users = other.users;
		operators = other.operators;
		bannedUsers = other.bannedUsers;
		topic = other.topic;
	}
	return *this;
}

Channel::~Channel(void) {
	return;
}

//broacasting methods

// Send a message to all users in the channel
void Channel::broadcast(const std::string& message, User* sender) {
	for (std::vector<User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
		User* user = *it;


		// Format the message according to the IRC protocol
		std::string formatted_message = ":" + sender->getName() + " PRIVMSG " + name + message + "\r\n";
        std::cerr << "|" << message << std::endl; 
		
		if (user != sender && user->isAlive) {
			send(user->getFd(), formatted_message.c_str(), formatted_message.size(), MSG_NOSIGNAL);
		}
	}
}

void Channel::broadcast_info(const std::string& info) {
    for (std::vector<User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
        User* user = *it;
        
        if (user->isAlive) {
            send(user->getFd(), info.c_str(), info.size(), MSG_NOSIGNAL);
        }
    }
}

// Send new topic to all users in the channel
void Channel::broadcast_new_topic( void ) {
    for (std::vector<User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
        User* user = *it;
		
        // Format the message according to the IRC protocol
        std::string msg = RPL_TOPIC(user, name, topic);

        if (user->isAlive) {
            send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
        }
    }
}

std::string Channel::who(User *requester) {
		std::stringstream ss;
		std::vector<User*>::iterator it;
		for (it = users.begin(); it != users.end(); ++it) {
			if ((*it)->visible || (*it) == requester || is_operator(requester) || requester->isServerOperator) {
				ss << ":" << requester->getName() << " 352 " << requester->getNick() << " "
				<< getName() << " " << (*it)->getName() << " " << (*it)->getHost()
				<< " " << (*it)->getName() << " H" << " :0 " << (*it)->getRealName() << "\r\n";
			}
		}
		ss << ":" << requester->getName() << " 315 " << requester->getNick() << " "
		<< getName() << " :End of /WHO list.\r\n";
		return ss.str();
}

////////// getters //////////



std::string Channel::get_topic(void) {
	return topic;
}

std::string Channel::getName( void ) {
    return ( this->name );
}

// Check if a user is in the channel
bool Channel::has_user(User* user) const {
	if (user)
    	return std::find(users.begin(), users.end(), user) != users.end();
	return false;
}

// Check if a user is invited to the channel
bool Channel::isInvited(User* user) {
	if (user)
    	return std::find(invitedUsers.begin(), invitedUsers.end(), user) != invitedUsers.end();
	return false;
}

// Check if a user is an operAtor in the channel
bool Channel::is_operator(User* user) const {
    return std::find(operators.begin(), operators.end(), user) != operators.end();
}

bool Channel::is_operator_empty(void) const {
	return operators.empty();
}

// Check if a user is  in the channel
User* Channel::find_user_by_nickname(const std::string& nickname) {
    for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
        if ((*it)->getNick() == nickname) {
            return *it;
        }
    }
    return NULL;
}

////////// setters //////////

void Channel::set_topic( std::string new_topic ) {
	if (!topic.empty())
		topic.erase();
	topic = new_topic;
}

// Add a user to the channel
void Channel::join(User* user) {
	if (this->isInviteOnly) {
		// If the user is invited, remove them from the invited list 
    invitedUsers.erase(std::remove(invitedUsers.begin(), invitedUsers.end(), user), invitedUsers.end());
	}
    users.push_back(user);
    std::string info = ":" + user->getName() + " JOIN " + name + "\r\n";
	std::cerr << std::endl << info << std::endl << std::endl;
    broadcast_info(info);
}

void Channel::modeInvite(User *user) {
	this->isInviteOnly = true;
	std::string info = ":" + user->getName() + " sets mode +i " + name + "\r\n";
	std::cerr << std::endl << info << std::endl << std::endl;
    broadcast_info(info);
}

void Channel::unModeInvite(User *user) {
	this->isInviteOnly = false;
	std::string info = ":" + user->getName() + " sets mode -i " + name + "\r\n";
	std::cerr << std::endl << info << std::endl << std::endl;
    broadcast_info(info);
}

void Channel::update_modes(const std::string &mode_changes, User *user, User *operAtor) {
        bool add_mode = true;
        std::cerr << std::endl << mode_changes << std::endl << std::endl;
        for (std::string::const_iterator it = mode_changes.begin(); it != mode_changes.end(); ++it) {
            char mode = *it;
            switch (mode) {
                case '+':
                    add_mode = true;
                    break;
                case '-':
                    add_mode = false;
                    break;
                case 'o':  // channel IRC operAtor mode
                    if (add_mode) {
                        add_to_operators(user, operAtor);
                        std::cerr << std::endl << user->getNick() << "was added to operators" << std::endl << std::endl;
                    } else {
                        remove_from_operators(user, operAtor);
                        std::cerr << std::endl << user->getNick() << "was removed from operators" << std::endl << std::endl;
                    }
                    break;
                case 'b':  // channel IRC ban mode
                    if (add_mode) {
                        add_to_ban(user, operAtor);
                        std::cerr << std::endl << user->getNick() << "was banned" << std::endl << std::endl;
                    } else {
                        remove_from_ban(user, operAtor);
                        std::cerr << std::endl << user->getNick() << "was unbanned" << std::endl << std::endl;
                    }
                    break;
				case 'i':  // channel IRC ban mode
                    if (add_mode) {
                        modeInvite(user);
                    } else {
                        unModeInvite(user);
                    }
                    break;
                // ... handle other user modes ...
                default:
                    // Ignore unsupported or unknown modes
                    break;
            }
        }
    }

bool Channel::isBanned(User *user) {
    for (std::vector<User*>::iterator it = bannedUsers.begin(); it != bannedUsers.end(); ++it) {
        if ((*it) == user) {
            std::string msg = ERR_BANNEDFROMCHAN(user, this);
            send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
            return true;
        }
    }
    return false;
}

void Channel::add_to_ban(User * user, User *operAtor) {
    for (std::vector<User*>::iterator it = bannedUsers.begin(); it != bannedUsers.end(); ++it) {
        if ((*it) == user) {
            return ;
        }
    }
    bannedUsers.push_back(user);
    this->kick(operAtor, user , "User has been banned from channel");
    std::string msg = ERR_BANNEDFROMCHAN(user, this);
    send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
}

void Channel::remove_from_ban(User * user, User *operAtor) {
    for (std::vector<User*>::iterator it = bannedUsers.begin(); it != bannedUsers.end(); ++it) {
        if ((*it) == user) {
            bannedUsers.erase(it);
            std::string msg = RPL_UNBANUSER(user, this, operAtor);
            send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
            return ;
        }
    }
}

void Channel::add_to_operators(User * user, User *operAtor) {
    for (std::vector<User*>::iterator it = operators.begin(); it != operators.end(); ++it) {
        if ((*it) == user) {
            return ;
        }
    }
    operators.push_back(user);
    std::string msg = RPL_ADDEDCHANOPER(user, this->name, operAtor);
    send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
}

void Channel::remove_from_operators(User * user, User *operAtor) {
    for (std::vector<User*>::iterator it = operators.begin(); it != operators.end(); ++it) {
        if ((*it) == user) {
            operators.erase(it);
            std::string msg = RPL_REMOVEDCHANOPER(user, this->name, operAtor);
            send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
            return ;
        }
    }
}

// Remove a user from the channel
void Channel::part(User* user) {
    users.erase(std::remove(users.begin(), users.end(), user), users.end());
    // If the user is an operAtor, remove them from the operators list as well
    operators.erase(std::remove(operators.begin(), operators.end(), user), operators.end());
    std::string info = ":" + user->getName() + " PART " + name + " " + user->getNick() +"\r\n";
    broadcast_info(info);
	if (user->isAlive) {
		std::string msg = ":" + user->getName() + " PART " + name + "\r\n";
		send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
	}
    // Assign operAtor privileges to another user if the channel is not empty
    if (!users.empty() && operators.empty()) {
        for (std::vector<User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
            User* target = *it;
            if (target->isAlive)
                add_to_operators(target, user);
        }
    }
}

void Channel::invite(User *user, User *targetUser) {

	if (has_user(targetUser)) {
		std::string msg = ERR_USERONCHANNEL(user, targetUser->getNick(), getName());
		send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
	} else if (!isInvited(targetUser)) {
		if (isBanned(targetUser))
			bannedUsers.erase(std::remove(bannedUsers.begin(), bannedUsers.end(), user), bannedUsers.end());
		std::string msg1 = ":" + user->getHost() + " 443 " + user->getNick() + " " + getName() + ":" + targetUser->getNick() + " has been invited to join " + getName() + "\n";
		send(targetUser->getFd(), msg1.c_str(), msg1.length(), MSG_NOSIGNAL);
		std::string msg = ":" + user->getName() + " INVITE " + targetUser->getNick() + " " + getName() + "\n";
		send(targetUser->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
		invitedUsers.push_back(targetUser);
	} else {
		std::string msg = ":" + user->getHost() + " 443 " + user->getNick() + " " + getName() + ":" + targetUser->getNick() + " has already been invited to join " + getName() + "\n";
		send(targetUser->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
	}
}

void Channel::kick(User *source, User *target, const std::string &reason) {
    if (has_user(target)) {
        // Remove the target from the channel's user list
        users.erase(std::remove(users.begin(), users.end(), target), users.end());
    	// If the target is an operAtor, remove them from the operators list as well
    	operators.erase(std::remove(operators.begin(), operators.end(), target), operators.end());

        // Format the kick message
        std::string kickMessage = ":" + source->getName() + " KICK " + name + " " + target->getNick();

        if (!reason.empty()) {
            kickMessage += " :" + reason;
        }

        kickMessage += "\r\n";

        // Send the kick message to all users in the channel
        broadcast_info(kickMessage);

        // Send the kick message to the kicked user
        send(target->getFd(), kickMessage.c_str(), kickMessage.length(), MSG_NOSIGNAL);

        // If the target user was an operAtor, remove them from the operAtor list
        if (!users.empty() && operators.empty()) {
			for (std::vector<User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
				User* user = *it;
				if (user->isAlive)
					add_to_operators(user, source);
            }
        }
    }
}
