/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpol <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 15:08:36 by rpol              #+#    #+#             */
/*   Updated: 2023/03/23 17:46:25 by rpol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "irc.hpp"
#include "macro.hpp"
#include <list>
#include "Channel.hpp"
class User;
class Channel;

class Server {

	public :
		Server( const Server & to_copy );
		Server & operator=( const Server & toTheRight );
		~Server( void );
		Server( std::string const password, std::string const hostName, int socket);
    
		////////// getters //////////
		std::string 					getPassword( void ) const;
		std::string 					getHostName( void ) const;
		std::string 					getStartDate( void ) const;
		int								getSocket( void ) const;
		int								getClientNbr( void ) const;
		
		pollfd							*getAllPollfds( void ) const;
		pollfd							getPollfd( int index ) const;
		
		std::list<Channel *>::iterator	find_channel( std::string channel_name );
		std::list<User*>::iterator		getUsersBegin( void );
		std::list<User*>::iterator		getUsersEnd( void );
		std::list<Channel *>::iterator	getChannelsBegin( void );
		std::list<Channel *>::iterator	getChannelsEnd( void );
		
		User 							*get_user_by_nickname(const std::string& nickname);
		User 							*get_user_by_index( int index );
		
		bool							is_valid_oper(std::string & username, std::string & password);

		////////// setters //////////
		void	setPollfd( int index, int fd_value, int events_value, int revents_value);
		void	clientNbrIncr( void );
		
		void	remove_channel(Channel* channel);
		void	add_to_Channels( Channel * channel);

		////////// methods //////////
		void	send_private_message(User *user, const std::string &recipient_nickname, const std::string &message);
		int		server_loop( void );
		int		bind_and_listen( int listening, int port);
		void	send_notice(const std::string& sender_nickname, const std::string& receiver_nickname, const std::string& message, bool broadcast = false);
		void  broadcastKill(User *sender);
		
		////////// variables //////////
		static Server* instance;

	private:
		Server( void );
		
		std::list< User * > users;
		std::list< Channel * > channels;
		sockaddr_in serverAddress;
		
		std::string		_adminUsername;
		std::string		_adminPassword;
		std::string		_password;
		std::string		_hostName;
		int				_socket;
		int				_clientNbr;
		pollfd			*_poll_fds;
		int				is_running;
		std::string		_startDate;
};



#endif