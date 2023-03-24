
#include "irc.hpp"
#include "Server.hpp"


////////// methods //////////
Server::Server( void ) {
	return;
}

Server::Server( const Server & to_copy ) {
	*this = to_copy;
	return;
}

Server &Server::operator=( const Server & toTheRight ) {
	if (this == &toTheRight)
		return (*this);
	this->_password = toTheRight._password;
	this->_hostName = toTheRight._hostName;
	this->_socket = toTheRight._socket;
	this->users = toTheRight.users;
	return (*this);
}

Server::Server( std::string const hostName, std::string const password, int fd ) {
	this->_password = password;
	this->_hostName = hostName;
	this->_socket = fd;
	this->_poll_fds = new pollfd[1024];
	this->_poll_fds[0].fd = fd;
	this->_poll_fds[0].events = POLLIN;
	this->_clientNbr = 0;
	this->_adminUsername = "ADMIN";
	this->_adminPassword = "ADMIN";
	this->is_running = true;
	
	time_t      stime = time(NULL);
    struct tm   *transTime;
    transTime = localtime(&stime);
	this->_startDate = std::string(asctime(transTime));
	return;
}



Server::~Server( void ) {
  this->is_running = false;
	for (std::list<User*>::iterator it = this->users.begin(); it != this->users.end(); ++it) {
		delete (*it);
	}
	for (std::list<Channel*>::iterator it = this->channels.begin(); it != this->channels.end(); ++it) {
		delete (*it);
	}
	delete[] this->_poll_fds;
	return;
}

int	Server::server_loop( void )
{
	Server *srv = this;
	char buff[4096];
	socklen_t	serverAddressLen = sizeof(this->serverAddress);
	int	current_client_fd;
	int client_index = 1;
	while (this->is_running) {

		signal(SIGINT, signalHandler);
		if (client_index > srv->getClientNbr()) {
			client_index = 1;
		}
		if (poll(srv->getAllPollfds(), srv->getClientNbr() + 1, 10 ) < 0) {
			std::cerr << "Error : cannot create new poll" << std::endl;
			return (EXIT_FAILURE);
		}
		if (srv->getPollfd(0).revents & POLLIN) {
			current_client_fd = accept(srv->getSocket(),
									  reinterpret_cast< sockaddr * >(&this->serverAddress),
									  &serverAddressLen);
			if (current_client_fd < 0) {
				
				std::cerr << "Error : cannot accept new client" << std::endl;
				return (EXIT_FAILURE);
			
			} else {
				
				srv->users.push_back(new User( current_client_fd ));
				srv->clientNbrIncr();
				srv->setPollfd(srv->getClientNbr(), current_client_fd, POLLIN, 0);
				std::cout << "New connection :" << srv->getClientNbr() << std::endl;
			}
		}
		if (srv->getClientNbr()) {
			if (srv->getPollfd(client_index).revents & POLLIN && srv->get_user_by_index(client_index)->isAlive) {	
				int bytesRecv = recv(srv->getPollfd(client_index).fd, buff, 4096, 0);
				if (bytesRecv < 0) {

					std::cout << " Client " << client_index << " disconected" << std::endl;
					srv->get_user_by_index(client_index)->isAlive = false;
				} else if (bytesRecv != 0) {
					
					std::string strBuff = std::string(buff, 0, bytesRecv);
					std::cout << "Received : [" << strBuff << "] from user " << client_index <<std::endl;
					srv->get_user_by_index(client_index)->appendBuff(strBuff);
				} else {
					srv->get_user_by_index(client_index)->isAlive = false;
				}
			}
			if (!srv->users.empty() && srv->get_user_by_index(client_index)->isAlive) {
				stream(client_index, *srv);
			}
		}
		memset(buff, 0, sizeof(buff));
		client_index++;
	}
	return EXIT_SUCCESS;
}

int	Server::bind_and_listen( int listening, int port ) {
	this->serverAddress.sin_family = AF_INET;
	this->serverAddress.sin_port = htons(port);
	inet_pton(AF_INET, "0.0.0.0", &this->serverAddress.sin_addr);
	if (bind( listening, reinterpret_cast< sockaddr * >( &this->serverAddress ), sizeof( this->serverAddress )) < 0 ) {
		std::cerr << "Error : cannot bind to IP/Port." << std::endl;
		return (EXIT_FAILURE);
	}

	if ( listen(listening, SOMAXCONN) < 0 ) {
		std::cerr << "Error : cannot listen to the IP/Port." << std::endl;
		return (EXIT_FAILURE);
	}
	return (0);
}

std::string Server::getStartDate(void) const
{
	return this->_startDate;
}

void Server::send_private_message(User *user, const std::string& recipient_nickname, const std::string& message) {
    User* recipient = get_user_by_nickname(recipient_nickname);

    if (recipient) {
      if (recipient->isAlive) {
        std::string formatted_message = ":" + user->getName() + " PRIVMSG " + recipient->getNick() + message + "\r\n";
        send(recipient->getFd(), formatted_message.c_str(), formatted_message.size(), MSG_NOSIGNAL);
      } else {
        std::string msg = ERR_NOSUCHNICK(user, recipient_nickname);
        send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
      }
    } else {
		std::string msg = ERR_NOSUCHNICK(user, recipient_nickname);
		send(user->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
	}

    
}



////////// getters //////////



std::string Server::getPassword( void ) const {
	return (this->_password);
}

std::string Server::getHostName( void ) const {
	return (this->_hostName);
}

pollfd		*Server::getAllPollfds( void ) const {
	return (this->_poll_fds);
}

int			Server::getClientNbr( void ) const {
	return (this->_clientNbr);
}

int			Server::getSocket( void ) const {
	return (this->_socket);
}

pollfd		Server::getPollfd( int index ) const {
	return (this->_poll_fds[index]);
}

User * Server::get_user_by_index( int userIndex ) {
	int index = 1;

	std::list< User * >::iterator it = this->users.begin();
	std::list< User * >::iterator ite = this->users.end();
	while (index != userIndex) {
		
		index++;
		it++;
		if (it == ite)
			std::cerr << "Error : User in Server ite problem" << std::endl;
	}
	return (*it);
}

User* Server::get_user_by_nickname(const std::string& nickname) {
    for (std::list<User*>::iterator it = users.begin(); it != users.end(); ++it) {
        if ((*it)->getNick() == nickname) {
            return *it;
        }
    }
    return NULL;
}

void Server::send_notice(const std::string& sender_nickname, const std::string& receiver_nickname, const std::string& message, bool broadcast)
{
    if (broadcast)
    {
        std::string notice_msg = ":" + sender_nickname + " NOTICE " + receiver_nickname + " :" + message + "\r\n";
        for (std::list<User*>::iterator it = users.begin(); it != users.end(); ++it)
        {
            User* user = *it;
            if (user->getNick() != sender_nickname && user->isAlive) {
                send(user->getFd(), notice_msg.c_str(), notice_msg.length(), MSG_NOSIGNAL);
            }
        }
    }
    else
    {
        User* receiver = get_user_by_nickname(receiver_nickname);

        if (receiver)
        {
            std::string notice_msg = ":" + sender_nickname + " NOTICE " + receiver_nickname + " :" + message + "\r\n";
            send(receiver->getFd(), notice_msg.c_str(), notice_msg.length(), MSG_NOSIGNAL);
        }
        else
        {
            User* sender = get_user_by_nickname(sender_nickname);
            std::string err_msg = ERR_NOSUCHNICK(sender, receiver_nickname);
            if (sender)
            {
                send(sender->getFd(), err_msg.c_str(), err_msg.length(), MSG_NOSIGNAL);
            }
        }
    }
}



std::list<User *>::iterator	 Server::getUsersBegin( void ) {
	return users.begin();
}

std::list<User *>::iterator	 Server::getUsersEnd( void ) {
	return users.end();
}

std::list<Channel *>::iterator	 Server::getChannelsBegin( void ) {
	return channels.begin();
}

std::list<Channel *>::iterator	 Server::getChannelsEnd( void ) {
	return channels.end();
}

std::list< Channel * >::iterator Server::find_channel( std::string channel_name ) {
	

	std::list< Channel * >::iterator it = this->channels.begin();
	std::list< Channel * >::iterator ite = this->channels.end();
	while (it != ite) {
		Channel * tmp = *it;
		if ( tmp->getName() == channel_name )
			return ( it );
		it++;
	}
	return (it);
}

bool Server::is_valid_oper(std::string &username, std::string &password)
{
	if ( username == this->_adminUsername && password == this->_adminPassword )
		return true;
	return false;
}

////////// setters //////////

void		Server::clientNbrIncr( void ) {
	this->_clientNbr += 1;
}

void Server::add_to_Channels(Channel* channel) {
	this->channels.push_back(channel);
}

void		Server::setPollfd( int index, int fd_value, int events_value, int revents_value) {
	this->_poll_fds[index].fd = fd_value;
	this->_poll_fds[index].events = events_value;
	this->_poll_fds[index].revents = revents_value;

}

void Server::remove_channel(Channel* channel) {
    std::list<Channel*>::iterator it = std::find(channels.begin(), channels.end(), channel);
    if (it != channels.end()) {
        channels.erase(it);
        delete channel;
    }
}

void Server::broadcastKill(User *sender) {
	for (std::list<User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
        User *receiver = *it;
		
        // Format the message according to the IRC protocol
        std::string msg = ERR_SRVDEAD(sender, receiver);

        if (receiver->isAlive) {
            send(receiver->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
        }
    }
}
