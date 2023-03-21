#include "irc.hpp"

bool	is_string_digit( char *str ) {
	while (*str) {

		if (!isdigit(*str))
			return (EXIT_FAILURE);
		str++;
	}
	return (EXIT_SUCCESS);
}

bool	arguments_check( int argc, char *str ) {
	if (argc != 3) {

		std::cerr << "Error : must put 2 arguments." << std::endl;
		return (EXIT_FAILURE);
	}
	if (is_string_digit(str)) {

		std::cerr << "Error : first argument have to be only digits." << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void signalHandler(int signum) {
    if (signum == SIGINT) {
        // ctrl+C was pressed
		if (Server::instance) {
        	delete Server::instance;
		}
		std::cerr << std::endl << std::endl << "The server has shut down" << std::endl << std::endl;
        exit(signum);
    }
}