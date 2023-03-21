#include "irc.hpp"

void	privmsg_cmd(std::istringstream *iss, std::string word, User *user, Server &srv)
{
	*iss >> word; // Extract the target (channel or user nickname)
	// Check if the target is a channel (starts with a '#' character)
	if (word[0] == '#') {
		std::list<Channel*>::iterator it = srv.find_channel(word);
		if (it != srv.getChannelsEnd()) {
		// Broadcast the message to the channel
			if ((*it)->isBanned(user))
				return;
			getline(*iss, word, ':'); // Skip the colon before the message content
			std::string message;
			getline(*iss, message); // Read the rest of the message
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
			getline(*iss, word, ':'); // Skip the colon before the message content
			std::string message;
			getline(*iss, message); // Read the rest of the message
			// Remove any trailing newline characters from the message
			message.erase(std::remove(message.begin(), message.end(), '\n'), message.end());
			message.erase(std::remove(message.begin(), message.end(), '\r'), message.end());
			// Send the private message to the target user
			srv.send_private_message(user->getNick(), receiver_nickname, message);
		}
}