#include "irc.hpp"

void	privmsg_cmd(std::istringstream *iss, std::string word, User *user, Server &srv)
{
	if (*iss >> word) { // Extract the target (channel or user nickname)
		// Check if the target is a channel (starts with a '#' character)
		if (word[0] == '#') {
			std::list<Channel*>::iterator it = srv.find_channel(word);
			if (it != srv.getChannelsEnd()) {
			// Broadcast the message to the channel
				if ((*it)->isBanned(user) && !user->isServerOperator)
					return;
				if (!(*it)->has_user(user)) {
					std::string msg = ERR_NOSUCHUSERINCHANNEL(user, word, user->getNick());
					send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
					return;
				}
				getline(*iss, word, ':');
				std::string message;
				getline(*iss, message); // Read the rest of the message
				// Remove any trailing newline characters from the message
				message.erase(std::remove(message.begin(), message.end(), '\n'), message.end());
				message.erase(std::remove(message.begin(), message.end(), '\r'), message.end());
				(*it)->broadcast(word + message, user);
			} else {
				std::string msg = ERR_NOSUCHCHANNEL(user, word);
				send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
			}
		} else {
				// The target is a user nickname
				std::string receiver_nickname = word;
				getline(*iss, word, ':');
				std::string message;
				getline(*iss, message); // Read the rest of the message
				// Remove any trailing newline characters from the message
				message.erase(std::remove(message.begin(), message.end(), '\n'), message.end());
				message.erase(std::remove(message.begin(), message.end(), '\r'), message.end());
				// Send the private message to the target user
				if (message.empty()) {
					std::string msg = ERR_NEEDMOREPARAMS(user, "PRIVMSG");
					send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
				}
				srv.send_private_message(user, receiver_nickname, message);
		}
	} else {
		std::string msg = ERR_NEEDMOREPARAMS(user, "PRIVMSG");
		send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
	}	
}