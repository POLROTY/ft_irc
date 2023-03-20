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

void	sighandler(int signal)
{
	if (signal == SIGINT)
	{
		delete Server::instance;
		exit(1);
	}
}