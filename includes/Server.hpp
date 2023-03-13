/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfascia <nathanfascia@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 15:08:36 by rpol              #+#    #+#             */
/*   Updated: 2023/03/13 18:31:15 by nfascia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "irc.hpp"
#include "macro.hpp"
#include <list>

class User;

class Server {

	public :
		//canon
		Server( const Server & to_copy );
		Server & operator=( const Server & toTheRight );
		~Server( void );

		Server( std::string const password, std::string const hostName, int socket);
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

	private :
		//canon
		Server( void );

		std::string		_password;
		std::string		_hostName;
		int				_socket;
		int				_clientNbr;
		pollfd			*_poll_fds;
};

int	server_loop(Server *srv, sockaddr_in serverAddress);
int	bind_and_listen(sockaddr_in serverAddress, int listening, int port);

#endif