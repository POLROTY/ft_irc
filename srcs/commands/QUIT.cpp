#include "irc.hpp"

void quit_cmd(User *user, Server &srv) {
	user->isAlive = false;
	for (std::list<Channel *>::iterator it = srv.getChannelsBegin(); it != srv.getChannelsEnd(); ++it) {
		if ((*it)) {
			if ((*it)->has_user(user)) {
				(*it)->part(user);
				if ((*it)->is_operator_empty()) {
					srv.remove_channel((*it));
					quit_cmd(user, srv);
					return;
				}
			}
		}
    }
    return;
}