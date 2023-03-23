#include "irc.hpp"

void	part_cmd(std::istringstream *iss, std::string word, User *user, Server &srv)
{
	if (*iss >> word) { // Extract the channel name
		std::list<Channel*>::iterator it = srv.find_channel(word);
		if (it != srv.getChannelsEnd()) {
			// Remove the user from the channel
			Channel *channel = *it;
			if (channel->has_user(user)) {
				channel->part(user);
				if (channel->is_operator_empty())
					srv.remove_channel(channel);
			} else {
				std::string msg = ERR_NOSUCHUSERINCHANNEL(user, word, user->getNick());
				send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
			}
		} else {
			// Handle the case where the channel was not found
			std::string msg = ERR_NOSUCHCHANNEL(user, word);
			send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
		}
	} else {
		std::string msg = ERR_NEEDMOREPARAMS(user, "PART");
		send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
	}
}