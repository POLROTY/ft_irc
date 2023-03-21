/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfascia <nathanfascia@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 14:19:21 by rpol              #+#    #+#             */
/*   Updated: 2023/03/20 18:48:48 by nfascia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

////////// utils //////////
#include <iostream>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <list>
#include <sstream>
#include <algorithm>
#include <csignal>


////////// class ////////// 
#include "Server.hpp"
#include "User.hpp"
#include "macro.hpp"
class Server;

////////// utils //////////

bool	is_string_digit( char *str );
bool	arguments_check( int argc, char *str );
void 	signalHandler(int signum);

/////////// FUNCTIONS IN SRCS/COMMANDS/NICK.CPP //////////

bool	isValidNickname( const std::string & nickname );
bool	nickInUse( std::string nickToCheck, Server & srv );

/////////// FUNCTIONS IN SRCS/EXECUTE.CPP //////////

void	stream( int client_index, Server & srv ); 

#endif