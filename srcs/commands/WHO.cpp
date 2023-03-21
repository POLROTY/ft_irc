#include "irc.hpp"

void	who_cmd(std::istringstream *iss, std::string *word, User *user, Server &srv)
{
	 if (*iss >> *word) {
        if ((*word)[0] == '#') {
            std::list<Channel*>::iterator it = srv.find_channel(*word);
            if (it != srv.getChannelsEnd()) {
				if ((*it)->isBanned(user))
					return;
                std::string msg = (*it)->who(user);
                send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
            } else {
                std::string msg = ERR_NOSUCHCHANNEL(user, *word);
                send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
            }
        }
	}
}