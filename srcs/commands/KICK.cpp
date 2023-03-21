#include "irc.hpp"

void	kick_cmd(std::istringstream *iss, User *user, Server &srv) {
	std::string channelName, targetNick, reason;
	if (*iss >> channelName >> targetNick) {
		std::list<Channel*>::iterator it = srv.find_channel(channelName);
		if (it != srv.getChannelsEnd()) {
			Channel *channel = *it;
			if (channel->is_operator(user) || user->isServerOperator) {
				User *targetUser = srv.get_user_by_nickname(targetNick);
				if (targetUser) {
					if (channel->has_user(targetUser)) {
						if (getline(*iss, reason, ':')) {
							getline(*iss, reason);
							reason.erase(std::remove(reason.begin(), reason.end(), '\n'), reason.end());
							reason.erase(std::remove(reason.begin(), reason.end(), '\r'), reason.end());
						}
						channel->kick(user, targetUser, reason);
						if (channel->is_operator_empty())
							srv.remove_channel(channel);
					} else {
						std::string msg = ERR_NOSUCHUSERINCHANNEL(user, channelName, targetNick);
						send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
					}
				} else {
					std::string msg = ERR_NOSUCHNICK(user, targetNick);
					send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
				}
			} else {
				std::string msg = ERR_CHANOPRIVSNEEDED(user, channelName);
				send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
			}
		} else {
			std::string msg = ERR_NOSUCHCHANNEL(user, channelName);
			send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
		}
	} else {
		std::string msg = ERR_NEEDMOREPARAMS(user, "KICK");
		send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
	}
}