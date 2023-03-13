/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfascia <nathanfascia@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 15:09:26 by rpol              #+#    #+#             */
/*   Updated: 2023/03/13 17:23:33 by nfascia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/Server.hpp"
#include "Server.hpp"

//canon
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

	return;
}

std::string Server::getPassword( void ) const {
	
	return (this->_password);
}

std::string Server::getHostName( void ) const {
	
	return (this->_hostName);
}

Server::Server( std::string const hostName, std::string const password, int fd ) {
	
	this->_password = password;
	this->_hostName = hostName;
	this->_socket = fd;
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
