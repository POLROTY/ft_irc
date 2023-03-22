#include "irc.hpp"

void	kill_cmd(std::istringstream *iss, User *user, Server &srv) {
	std::string targetNickname, message;
	if (*iss >> targetNickname >> message) {
		if (!user->isServerOperator) {
			std::string msg = ERR_NOPRIVILEGES(user);
			send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
			return ;
		}
		if (targetNickname == "*SERVER*") {
			srv.broadcastKill(user);
			delete &srv;
			Server::instance = NULL;
			return ;
		}
		User* targetUser = srv.get_user_by_nickname(targetNickname);
		if (targetUser) {
			if (user->isServerOperator) {
				std::string msg = ERR_YOUDEAD(targetUser, user);
				send(targetUser->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
				targetUser->isAlive = false;
			} else {
				std::string msg = ERR_NOPRIVILEGES(user);
				send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
			}
		} else {
			std::string msg = ERR_NOSUCHNICK(user, targetNickname);
			send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
		}
	} else {
		std::string msg = ERR_NEEDMOREPARAMS(user, "KILL");
		send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
	}
}