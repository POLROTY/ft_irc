#include "irc.hpp"

void	mode_cmd(std::istringstream *iss, User *user, Server &srv) {
	std::string target, mode_changes, target_user_nick;
	if (*iss >> target >> mode_changes) {
		if (target[0] == '#') {  // Channel mode
			std::list<Channel*>::iterator it = srv.find_channel(target);
			if (it != srv.getChannelsEnd()) {
				Channel *channel = *it;
				if (channel->is_operator(user) || user->isServerOperator) {
					if ( *iss >> target_user_nick ) {
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
}