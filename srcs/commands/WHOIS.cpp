#include "irc.hpp"

void	whois_cmd(std::istringstream *iss, std::string word, User *user, Server &srv)
{
	if (*iss >> word) {
        User* targetUser = srv.get_user_by_nickname(word);
        if (targetUser) {
            std::string msg = RPL_WHOISUSER(user, targetUser) + RPL_WHOISSERVER(user, targetUser) + RPL_ENDOFWHOIS(user, targetUser);
            send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
        } else {
            std::string msg = ERR_NOSUCHNICK(user, word);
            send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
        }
    } else {
        std::string msg = ERR_NONICKNAMEGIVEN(user);
        send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
    }
}