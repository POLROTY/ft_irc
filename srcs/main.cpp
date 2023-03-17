
#include "../includes/macro.hpp"
#include "../includes/irc.hpp"
#include "../includes/User.hpp"


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

int main(int argc, char **argv){
	if (arguments_check(argc, argv[1]))
		return (EXIT_FAILURE);
	int listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening < 0) {
		std::cerr << "An error has occured while creating the socket." << std::endl;
		return (EXIT_FAILURE);
	}
	std::cout << "socket is : " << listening << std::endl;
	Server srv = Server("localhost", argv[2], listening);
	
	if (srv.bind_and_listen( listening, atoi(argv[1])) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (srv.server_loop() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}