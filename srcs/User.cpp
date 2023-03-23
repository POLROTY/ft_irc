/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpol <rpol@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 14:22:40 by rpol              #+#    #+#             */
/*   Updated: 2023/03/23 01:12:28 by rpol             ###   ########.fr       */
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
	this->isServerOperator = false;
	this->visible = true;
	this->_host = "localhost";
	this->_nick = "$";
	this->_name = "$";
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
	clearNick();
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

void User::update_modes(const std::string& mode_changes) {
        bool add_mode = true;
        for (std::string::const_iterator it = mode_changes.begin(); it != mode_changes.end(); ++it) {
            char mode = *it;
            switch (mode) {
                case '+':
                    add_mode = true;
                    break;
                case '-':
                    add_mode = false;
                    break;
                case 'o':  // server IRC operator mode
                    if (add_mode) {
                        std::string msg = ERR_PASSWDMISMATCH(this);
            			send(this->getFd(), msg.c_str(), msg.length(), MSG_NOSIGNAL);
                    } else {
                        this->isServerOperator = false;
                    }
                    break;
                case 'i':  // user visible
                    if (add_mode) {
                        this->visible = false;
						std::string msg1 = ":" + this->getHost()+ " 221 " + this->getNick() + " +i\r\n"; 
						send(this->getFd(), msg1.c_str(), msg1.length(), MSG_NOSIGNAL);
                    } else {
                        this->visible = true;
						std::string msg1 = ":" + this->getHost()+ " 221 " + this->getNick() + " -i\r\n"; 
						send(this->getFd(), msg1.c_str(), msg1.length(), MSG_NOSIGNAL);
                    }
                    break;
					
                default:
                    // Ignore unsupported or unknown modes
                    break;
            }
        }
    }