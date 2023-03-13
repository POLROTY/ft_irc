/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfascia <nathanfascia@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 14:22:40 by rpol              #+#    #+#             */
/*   Updated: 2023/03/13 18:28:59 by nfascia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

User::User( void ) {
	return;
}

User::User( int fd ) {
	this->_fd = fd;
	this->isUserSet = false;
	this->_isPasswordChecked = false;
	this->isAlive = true;
	return;
}

User::User( const User & to_copy ) {
	*this = to_copy;
	return;
}

User &User::operator=( const User & toTheRight ){
	if (this == &toTheRight)
		return (*this);
	this->_fd = toTheRight._fd;
	this->_nick = toTheRight._nick;
	this->_name = toTheRight._name;
	this->_mode = toTheRight._mode;
	this->_host = toTheRight._host;
	return (*this);
}

User::~User( void ) {
	return;
}

void User::printInfo( void ) {
	std::cout << _nick << std::endl << getName() << std::endl;	
}

void User::appendBuff( std::string str ) {
	if (this->_buff.empty()) {
		this->_buff = str;
	} else {
		this->_buff.append(str.c_str());
	}
}

void	User::initUser( std::string password ) {
	std::istringstream iss(this->getBuff());
    std::string word;
	while (iss >> word) {
		
		if (word == "PASS") {
			
			if (iss >> word) {

				if (word == password)
					this->_isPasswordChecked = true;
				else {
					this->isAlive = false;
					std::string msg = ERR_PASSWDMISMATCH(this);
					send(this->_fd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
				}
			}
		}		 
		else if (word == "USER") {
			
			if (!this->_isPasswordChecked)
				break;
			if (iss >> word)
			{
				this->_nick = word;
				if (iss >> word)
					this->_name = word;
				if (iss >> word) {
				
				this->_host = word;
				this->isUserSet = true;
				this->_buff.erase(0, 1 + _buff.find('\r', _buff.find(_host.c_str())));
				return;
				}
			}
		}
	}
	this->_buff.erase(this->_buff.length());
}

////////// getters //////////
std::string User::getNick( void ) const {
	return (this->_nick);
}

int User::getFd( void ) const {
	return (this->_fd);
}

std::string User::getName( void ) const {
	return (this->_nick + "!" + this->_name + "@" + this->_host);
}

void User::setNick( std::string nick ) {
	this->_nick = nick;
}

std::string User::getHost( void ) const {
	return (this->_host);
}

std::string User::getBuff( void ) const {
	return (this->_buff);
}

////////// setters //////////
void	User::setBuff( std::string newBuff ) {
	this->_buff = newBuff;
}