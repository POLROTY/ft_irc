/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpol <rpol@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 14:22:40 by rpol              #+#    #+#             */
/*   Updated: 2023/03/16 00:14:28 by rpol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

User::User( void ) {
	return;
}

User::User( int fd ) {
	this->_fd = fd;
	this->isUserSet = false;
	this->isIrssi = false;
	this->isPasswordChecked = false;
	this->isAlive = true;
	this->_host = "localhost";
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

// void	User::initUser( std::string password ) {

// 	std::string::size_type pos = this->getBuff().find_first_of('\n');
// 	std::string command = this->getBuff().substr(0, pos);
// 	this->setBuff( this->getBuff().erase(0, pos + 1) );
	
// 	std::istringstream iss( command );
//     std::string word;
// 	iss >> word;
	
// 	std::cerr << word << std::endl;
	
// 	if ( word == "CAP" && !this->_isPasswordChecked ) {
// 		iss >> word;
// 		if ( word == "LS" )
// 			this->isIrssi = true;
// 	} else if ( word == "PASS" ) {
		
// 			if ( word == password )
// 				this->_isPasswordChecked = true;
// 				std::cerr << "A user got password right" << std::endl;
// 			else {
// 				if ( this->isIrssi )
// 					this->isAlive = false;
// 				std::string msg = ERR_PASSWDMISMATCH(this);
// 				send(this->_fd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
// 			}
// 		}
// 	} else if (word == "NICK") {
		
// 		if (!this->_isPasswordChecked)
// 			break;
// 		if (iss >> word) {
// 				std::string str;
// 				if (nickInUse(word,  srv)) {

// 					str = ERR_NICKNAMEINUSE(user, word);
// 				} else {

// 					user->setNick(word);
// 					str = NICK(user, word);
// 				}
// 				send(user->getFd(), str.c_str(), str.length(), MSG_NOSIGNAL);
// 			}
// 	} else if ( word == "USER" ) {
		
// 		if (!this->_isPasswordChecked)
// 			break;
// 		if (iss >> word)
// 		{
			
// 			if (iss >> word)
// 				this->_name = word;
// 			if (iss >> word) {
			
// 			this->_host = word;
// 			if 
// 			this->isUserSet = true;
// 			return;
// 			}
// 		}
// 	}
	
// 	this->_buff.erase(this->_buff.length());
// }

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

std::string User::getRealName( void ) const {
	return ( this->_name );
}

void User::setNick( std::string nick ) {
	this->_nick = nick;
}

void User::clearNick( void ) {
	this->_nick.clear() ;
}

void User::setName( std::string Name ) {
	this->_name = Name;
}

void User::setHost( std::string Host ) {
	this->_host = Host;
}

void User::setMode( int mode ) {
	this->_mode = mode;
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