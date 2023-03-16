/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpol <rpol@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 15:09:26 by rpol              #+#    #+#             */
/*   Updated: 2023/03/15 21:53:38 by rpol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//canon

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

Server::~Server( void ) {
	delete[] this->_poll_fds;
	return;
}

Server::Server( std::string const hostName, std::string const password, int fd ) {
	this->_password = password;
	this->_hostName = hostName;
	this->_socket = fd;
	this->_poll_fds = new pollfd[1024];
	this->_poll_fds[0].fd = fd;
	this->_poll_fds[0].events = POLLIN;
	this->_clientNbr = 0;
	return;
}

User * Server::user( int userIndex ) {
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

////////// setters //////////
void		Server::clientNbrIncr( void ) {
	this->_clientNbr += 1;
}

void		Server::setPollfd( int index, int fd_value, int events_value, int revents_value) {
	this->_poll_fds[index].fd = fd_value;
	this->_poll_fds[index].events = events_value;
	this->_poll_fds[index].revents = revents_value;
}


////////// functions //////////


int	bind_and_listen( sockaddr_in serverAddress, int listening, int port ) {
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	inet_pton(AF_INET, "0.0.0.0", &serverAddress.sin_addr);
	if (bind( listening, reinterpret_cast< sockaddr * >( &serverAddress ), sizeof( serverAddress )) < 0 ) {
		std::cerr << "Error : cannot bind to IP/Port." << std::endl;
		return (EXIT_FAILURE);
	}

	if ( listen(listening, SOMAXCONN) < 0 ) {
		std::cerr << "Error : cannot listen to the IP/Port." << std::endl;
		return (EXIT_FAILURE);
	}
	return (0);
}

int	server_loop(Server *srv, sockaddr_in serverAddress)
{
	char buff[4096];
	socklen_t	serverAddressLen = sizeof(serverAddress);
	int	current_client_fd;
	int client_index = 1;
	while (true) {

		if (client_index > srv->getClientNbr()) {
			client_index = 1;
		}
		if (poll(srv->getAllPollfds(), srv->getClientNbr() + 1, 10 ) < 0) {
			std::cerr << "Error : cannot create new poll" << std::endl;
			return (EXIT_FAILURE);
		}
		if (srv->getPollfd(0).revents & POLLIN) {
			current_client_fd = accept(srv->getSocket(),
									  reinterpret_cast< sockaddr * >(&serverAddress),
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
		if (srv->getPollfd(client_index).revents & POLLIN && srv->user(client_index)->isAlive) {	
			int bytesRecv = recv(srv->getPollfd(client_index).fd, buff, 4096, 0);
			if (bytesRecv < 0) {

				std::cout << " Client " << client_index << " disconected" << std::endl;
				srv->user(client_index)->isAlive = false;
			} else if (bytesRecv != 0) {
				
				std::string strBuff = std::string(buff, 0, bytesRecv);
				std::cout << "Received : [" << strBuff << "] from user " << client_index <<std::endl;
				srv->user(client_index)->appendBuff(strBuff);
			} else {
				srv->user(client_index)->isAlive = false;
			}
		}
		if (!srv->users.empty() && srv->user(client_index)->isAlive) {
			stream(client_index, *srv);
		}
		memset(buff, 0, sizeof(buff));
		client_index++;
	}
}