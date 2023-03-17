#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "irc.hpp"
#include <list>
#include <algorithm>
class User;

class Channel {
public:
    // Default constructor
    Channel() {}

    // Parameterized constructor
    Channel(const std::string& name) : name(name) {}

    Channel( const std::string & name, User * user );

    // Copy constructor
    Channel(const Channel& other) : name(other.name), users(other.users), operators(other.operators) {}

    // Assignment operator
    Channel& operator=(const Channel& other) {
        if (this != &other) {
            name = other.name;
            users = other.users;
            operators = other.operators;
        }
        return *this;
    }

    // Destructor
    ~Channel() {}

    // Other methods...
    void join(User *user);
    std::string getName( void ) { return ( this->name ); }

	bool has_user(User *user) const;
	bool is_operator(User *user) const;
	void broadcast(const std::string &message, User *sender);
	void part(User *user);

private:
    std::string name;
    std::vector<User*> users;
    std::vector<User*> operators;
	void add_first_user_as_operator(User *user);
	
};

#endif