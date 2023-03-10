
#include "../includes/macro.hpp"
#include "../includes/irc.hpp"
#include "../includes/User.hpp"


bool	is_string_digit( char *str ) {

	while ( *str ) {

		if ( !isdigit( *str ) )
			return ( EXIT_FAILURE );
		str++;
	}
	return ( EXIT_SUCCESS );
}

bool	arguments_check( int argc, char *str ) {

	if ( argc != 3 ) {

		std::cerr << "Error : must put 2 arguments." << std::endl;
		return ( EXIT_FAILURE );
	}
	if ( is_string_digit( str ) ) {

		std::cerr << "Error : first argument have to be only digits." << std::endl;
		return ( EXIT_FAILURE );
	}
	return ( EXIT_SUCCESS );
}


void check_de_la_street( User *user ) {
	
	user->printInfo();
    send( user->getFd(), RPL_WELCOME(user).c_str(), RPL_WELCOME(user).length(), MSG_NOSIGNAL );
	send( user->getFd(), RPL_YOURHOST(user).c_str(), RPL_YOURHOST(user).length(), MSG_NOSIGNAL );
	send( user->getFd(), RPL_CREATED(user).c_str(), RPL_CREATED(user).length(), MSG_NOSIGNAL );
	send( user->getFd(), RPL_MYINFO(user).c_str(), RPL_MYINFO(user).length(), MSG_NOSIGNAL );
}

bool nickInUse( std::string nickToCheck, Server & srv ) {

	std::list< User * >::iterator it = srv.users.begin();
	std::list< User * >::iterator ite = srv.users.end();
	while ( it != ite ) {
		
		if ( nickToCheck == (*it)->getNick() )
			return ( true );
		it++;
	}
	return ( false );
}

void stream( int client_index, Server & srv ) {
	
	User * user = srv.user( client_index );

	size_t pos = user->getBuff().find('\r');
	if ( pos == std::string::npos )
		return;
	if ( !user->isUserSet ) {

		user->initUser( srv.getPassword() );
		if ( user->isUserSet )
			check_de_la_street( user );
		return;
	}
	std::istringstream iss( user->getBuff() );
    std::string word;

	if ( iss >> word ) {
		
		if ( word == "NICK") {

			if ( iss >> word ) {
				std::string str;
				if ( nickInUse( word,  srv )) {

					str = ERR_NICKNAMEINUSE( user, word );
				} else {

					user->setNick( word );
					str = NICK( user, word );
				}
				send( user->getFd(), str.c_str(), str.length(), MSG_NOSIGNAL );
			}
		}else if ( word == "PING") {

			if ( iss >> word ) {
				std::string str = PONG( user );
				send( user->getFd(), str.c_str(), str.length(), MSG_NOSIGNAL );
				std::cerr << str << std::endl;
			}
		} else {

			// std::string str = ":" + user->getName() + " 404 " + user->getNick() + " :" + user->getHost() + " UNKNOWN COMMAND YET\n" ;
			// send( user->getFd(), str.c_str(), str.length(), ERR_NOTIMPLEMENTED );
			send(user->getFd(),  ERR_NOTIMPLEMENTED(word).c_str(), ERR_NOTIMPLEMENTED(word).length(), MSG_NOSIGNAL);
		}
		user->setBuff( user->getBuff().erase( 0, pos + 1 ) );
	}
}

// bool passwordCheck( std::string str, Server & srv ) {

// 	std::istringstream iss( str );
//     std::string word;
// 	while ( iss >> word ) {
// 		std::cerr << "|" << word << "|" << std::endl;
// 		if ( word == "PASS" ) {
			
// 			if ( iss >> word ) {
// 				std::cerr << word << std::endl;
// 				std::cerr << srv.getPassword() << std::endl;
// 				if ( word == srv.getPassword() )
// 					return ( true );
// 			}
// 		}
// 	}
// 	return( false );
// }

int main(int argc, char **argv)
{

	if ( arguments_check(argc, argv[1]) )
		return ( EXIT_FAILURE );

	int listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening < 0) {

		std::cerr << "An error has occured while creating the socket." << std::endl;
		return ( EXIT_FAILURE );
	}
	std::cout << "socket is : " << listening << std::endl;
	
	Server *srv = new Server( "localhost", argv[2], listening );

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons( atoi( argv[1] ) );
	inet_pton( AF_INET, "0.0.0.0", &serverAddress.sin_addr );

	
	if ( bind( listening, reinterpret_cast< sockaddr * >( &serverAddress ), sizeof( serverAddress ) ) < 0 ) {

		std::cerr << "Error : cannot bind to IP/Port." << std::endl;
		return ( EXIT_FAILURE );
	}


	
	if ( listen( listening, SOMAXCONN ) < 0 ) {

		std::cerr << "Error : cannot listen to the IP/Port." << std::endl;
		return ( EXIT_FAILURE );
	}

	char buff[4096];
	

	pollfd		poll_fds[1024];
	poll_fds[0].fd = listening;
	poll_fds[0].events = POLLIN;

	socklen_t	serverAddressLen = sizeof( serverAddress );

	int	current_client_fd;
	int	number_of_clients = 0 ;
	int client_index = 1;
	while ( true ) {

		if ( client_index > number_of_clients ) {

			client_index = 1;
		}

		if ( poll( poll_fds, number_of_clients + 1, 10 ) < 0 ) {

			std::cerr << "Error : cannot create new poll" << std::endl;
			return ( EXIT_FAILURE );
		}
		
		if ( poll_fds[0].revents & POLLIN ) {
			
			current_client_fd = accept( listening,
									  reinterpret_cast< sockaddr * >( &serverAddress ),
									  &serverAddressLen );
			if ( current_client_fd < 0 ) {
				
				std::cerr << "Error : cannot accept new client" << std::endl;
				return ( EXIT_FAILURE );
			
			} else {
				
				srv->users.push_back( new User( current_client_fd ) );
				number_of_clients++;
				poll_fds[number_of_clients].fd = current_client_fd;
				poll_fds[number_of_clients].events = POLLIN; 
				std::cout << "New connection :" << number_of_clients << std::endl;
			}
		}

		if ( poll_fds[client_index].revents & POLLIN && srv->user( client_index )->isAlive ) {
			
			int bytesRecv = recv( poll_fds[client_index].fd, buff, 4096, 0);
			if ( bytesRecv < 0 ) {

				std::cout << " Client " << client_index << " disconected" << std::endl;
				srv->user( client_index )->isAlive = false;
			} else if ( bytesRecv != 0 ) {
				
				std::string strBuff = std::string( buff, 0, bytesRecv );
				std::cout << "Received : [" << strBuff << "] from user " << client_index <<std::endl;
				srv->user( client_index )->appendBuff( strBuff );
			} else {
				srv->user( client_index )->isAlive = false;
			}
		}
		
		if ( !srv->users.empty() && srv->user( client_index )->isAlive ) {
			stream( client_index, *srv );
		}

		memset( buff, 0, sizeof( buff ) );
		client_index++;
	}

	return ( EXIT_SUCCESS );
}