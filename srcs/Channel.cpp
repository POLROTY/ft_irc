#include "Channel.hpp"
#include <algorithm>
#include <sys/socket.h>

// Add the first user to the channel and mark them as an operator
void Channel::add_first_user_as_operator(User* user) {
    if (users.empty()) {
        operators.push_back(user); // Add the first user as an operator
        users.push_back(user);
    }
}

// Add a user to the channel
void Channel::join(User* user) {
    users.push_back(user);
}


Channel::Channel( const std::string & name, User * user) : name(name) {
    users.push_back( user );
    operators.push_back( user ); // Add the first user as an operator
}

// Remove a user from the channel
void Channel::part(User* user) {
    users.erase(std::remove(users.begin(), users.end(), user), users.end());
    // If the user is an operator, remove them from the operators list as well
    operators.erase(std::remove(operators.begin(), operators.end(), user), operators.end());

    // Assign operator privileges to another user if the channel is not empty
    if (!users.empty() && operators.empty()) {
        operators.push_back(users.front());
    }
}

// Check if a user is in the channel
bool Channel::has_user(User* user) const {
    return std::find(users.begin(), users.end(), user) != users.end();
}

// Check if a user is an operator in the channel
bool Channel::is_operator(User* user) const {
    return std::find(operators.begin(), operators.end(), user) != operators.end();
}

// Send a message to all users in the channel
void Channel::broadcast(const std::string& message, User* sender) {
    for (std::vector<User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
        User* user = *it;
        if (user != sender) {
            send(user->getFd(), message.c_str(), message.size(), 0);
        }
    }
}
