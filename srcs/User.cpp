/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpol <rpol@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 14:22:40 by rpol              #+#    #+#             */
/*   Updated: 2023/03/20 23:45:37 by rpol             ###   ########.fr       */
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

void User::setMode( std::string channel, std::string mode ) {
	this->channelModes.push_back(channel + mode);
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
                case 'o':  // channel IRC operator mode
                    if (add_mode) {
                        //send message to say you cant
                    } else {
                        this->isServerOperator = false;
                    }
                    break;
                // case 'b':  // channel IRC ban mode
                //     if (add_mode) {
                //         add_to_ban(user);
                //     } else {
                //         remove_from_ban(user);
                //     }
                //     break;
                // ... handle other user modes ...
                default:
                    // Ignore unsupported or unknown modes
                    break;
            }
        }
    }