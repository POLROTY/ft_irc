/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfascia <nathanfascia@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 14:19:21 by rpol              #+#    #+#             */
/*   Updated: 2023/03/22 17:47:59 by nfascia          ###   ########.fr       */
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


////////// CLASS ////////// 

#include "Server.hpp"
#include "User.hpp"
#include "macro.hpp"
class Server;

////////// UTILS //////////

bool	is_string_digit( char *str );
bool	arguments_check( int argc, char *str );
void 	signalHandler(int signum);

/////////// CMD FUNCTIONS //////////

void	handshake(User *user);
bool	isValidNickname( const std::string & nickname );
bool	nickInUse( std::string nickToCheck, Server & srv );
void	stream( int client_index, Server & srv );
void	cap_cmd(std::istringstream *iss, std::string word);
void	ls_cmd(User *user);
void	pass_cmd(std::istringstream *iss, std::string *word, User *user, Server &srv);
void	nick_cmd(std::istringstream *iss, std::string *word, User *user, Server &srv);
void	user_cmd(std::istringstream *iss, std::string *word, User *user);
void	ping_cmd(std::istringstream *iss, std::string word, User *user);
void	who_cmd(std::istringstream *iss, std::string *word, User *user, Server &srv);
void	oper_cmd(std::istringstream *iss, User *user, Server &srv);
void	whois_cmd(std::istringstream *iss, std::string word, User *user, Server &srv);
void	join_cmd(std::istringstream *iss, std::string word, User *user, Server &srv);
void	part_cmd(std::istringstream *iss, std::string word, User *user, Server &srv);
void	topic_cmd(std::istringstream *iss, std::string word, User *user, Server &srv);
void	privmsg_cmd(std::istringstream *iss, std::string word, User *user, Server &srv);
void	invite_cmd(std::istringstream *iss, std::string word, User *user, Server &srv);
void	kick_cmd(std::istringstream *iss, User *user, Server &srv);
void	kill_cmd(std::istringstream *iss, User *user, Server &srv);
void	mode_cmd(std::istringstream *iss, User *user, Server &srv);
void	quit_cmd(User *user, Server &srv);

#endif