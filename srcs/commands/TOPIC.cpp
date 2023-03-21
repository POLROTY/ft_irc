#include "irc.hpp"

void	topic_cmd(std::istringstream *iss, std::string word, User *user, Server &srv)
{
	if (*iss >> word) { // Extract the channel name
		std::list<Channel*>::iterator it = srv.find_channel(word);
		if (it != srv.getChannelsEnd()) {
			std::string new_topic;
			if ((*it)->isBanned(user))
				return;
			if (getline(*iss, new_topic, ':')) { // If a new topic is specified
				getline(*iss, new_topic); // Read the rest of the new topic
				// Remove any trailing newline characters from the new topic
				if (!(*it)->is_operator(user)){
					std::string msg = ERR_CHANOPRIVSNEEDED(user, (*it)->getName());
					send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
				} else {
				new_topic.erase(std::remove(new_topic.begin(), new_topic.end(), '\n'), new_topic.end());
				new_topic.erase(std::remove(new_topic.begin(), new_topic.end(), '\r'), new_topic.end());
				// Set the new topic for the channel
				(*it)->set_topic(new_topic);
				(*it)->broadcast_new_topic();
				}
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
}