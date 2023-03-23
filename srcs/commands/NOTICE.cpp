#include "irc.hpp"


void notice_cmd(std::istringstream *iss, std::string word, User *user, Server &srv)
{
    if (*iss >> word) { 
		bool broadcast = false;
		if (word[0] == '#')
			broadcast = true;
		std::string receiver_nickname = word;
		getline(*iss, word, ':'); 
		std::string message;
		getline(*iss, message); 
		message.erase(std::remove(message.begin(), message.end(), '\n'), message.end());
		message.erase(std::remove(message.begin(), message.end(), '\r'), message.end());
		// Send the notice to the target user
		srv.send_notice(user->getNick(), receiver_nickname, message, broadcast);
	} else {
		std::string msg = ERR_NEEDMOREPARAMS(user, "NOTICE");
		send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
	}
}
