#include "irc.hpp"

void	oper_cmd(std::istringstream *iss, User *user, Server &srv)
{
	std::string username, password;
    if (*iss >> username >> password) {
    	if (srv.is_valid_oper(username, password)) {
            user->isServerOperator = true;
            std::string msg = RPL_YOUREOPER(user);
            send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
        } else {
            std::string msg = ERR_PASSWDMISMATCH(user);
            send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
        }
    } else {
        std::string msg = ERR_NEEDMOREPARAMS(user, "OPER");
        send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
	}
}