#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "irc.hpp"
#include <list>
#include <algorithm>
class User;

class Channel {
public:
    


    Channel( const std::string & name, User * user );

    // Copy constructor
    Channel(const Channel& other);

    // Assignment operator
    Channel& operator=(const Channel& other); 

    // Destructor
    ~Channel( void );


    ///////// getters /////////


    std::string getName( void );
    std::string get_topic( void );
    User* find_user_by_nickname(const std::string& nickname);
    bool has_user(User *user) const;
    bool is_operator(User *user) const;
    bool is_operator_empty( void ) const;
    bool isBanned(User *user);
    



    ///////// setters /////////

    void join(User *user);
    void update_modes(const std::string& mode_changes, User *user);
    void set_topic( std::string new_topic );

    //Operator methods
    
    void add_to_operators( User *user);
    void remove_from_operators(User *user);

    void add_to_ban(User *user);
    void remove_from_ban(User *user);


	//broacasting methods
	
	void broadcast_new_topic( void );
    void broadcast(const std::string &message, User *sender);
    void broadcast_info(const std::string& info);
	void part(User *user);
    std::string who(User *requester);
	void invite(User *user, User *targetUser);
	void kick(User *source, User *target, const std::string &reason);

private:

	// Default constructor
    Channel( void );

    std::string topic;
    std::string name;
    std::vector<User*> users;
    std::vector<User*> operators;
    std::vector<User*> bannedUsers;
	
};

#endif