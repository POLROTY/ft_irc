#include "irc.hpp"

void	invite_cmd(std::istringstream *iss, std::string word, User *user, Server &srv) {
	std::string targetUserNick;
	if (*iss >> targetUserNick) { // Extract the user nickname to be invited
		if (*iss >> word) { // Extract the channel name
			std::list<Channel*>::iterator it = srv.find_channel(word);
			if (it != srv.getChannelsEnd()) {
				// Check if the user is in the channel
				Channel *channel = *it;
				if (channel->isBanned(user))
					return;
				if (channel->has_user(user) || user->isServerOperator) {
					// Check if the user has operator status in the channel
					if (channel->is_operator(user) || user->isServerOperator) {
						User* targetUser = srv.get_user_by_nickname(targetUserNick);
						if (targetUser) {
							// Invite the user to the channel
							channel->invite(user, targetUser);
						} else {
							std::string msg = ERR_NOSUCHNICK(user, word);
							send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
						}
					} else {
						std::string msg = ERR_CHANOPRIVSNEEDED(user, word);
						send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
					}
				} else {
					std::string msg = ERR_NOTONCHANNEL(user, word);
					send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
				}
			} else {
				std::string msg = ERR_NOSUCHCHANNEL(user, word);
				send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
			}
		}
	}
}