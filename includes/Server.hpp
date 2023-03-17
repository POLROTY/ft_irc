/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpol <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 15:08:36 by rpol              #+#    #+#             */
/*   Updated: 2023/03/17 18:13:01 by rpol             ###   ########.fr       */
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
		//canon
		Server( const Server & to_copy );
		Server & operator=( const Server & toTheRight );
		~Server( void );

		Server( std::string const password, std::string const hostName, int socket);
		int	server_loop( void );
		int	bind_and_listen( int listening, int port);
		
		std::string getPassword( void ) const;
		std::string getHostName( void ) const;
		pollfd		*getAllPollfds( void ) const;
		pollfd		getPollfd( int index ) const;
		int			getSocket( void ) const;
		int			getClientNbr( void ) const;
		void		clientNbrIncr( void );
		void		setPollfd( int index, int fd_value, int events_value, int revents_value);
		User 		*user( int index );
		std::list< User * > users;
		std::list< Channel * > channels;
		std::list< Channel * >::iterator find_channel( std::string channel_name );
		User* find_user_by_nickname(const std::string& nickname);
		void send_private_message(const std::string &sender_nickname, const std::string &recipient_nickname, const std::string &message);
		sockaddr_in serverAddress;
	private:
		//canon
		Server( void );
		std::string		_password;
		std::string		_hostName;
		int				_socket;
		int				_clientNbr;
		pollfd			*_poll_fds;
};



#endif