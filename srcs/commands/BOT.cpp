#include "irc.hpp"

void	bot(User *user, std::string &input, Server &srv) {

	std::vector<std::string> jokes;
    srand(time(NULL));
    int index = -1;
    
    // Add jokes to the vector
    jokes.push_back("Why did the tomato turn red? Because it saw the salad dressing!");
    jokes.push_back("Why did the chicken cross the playground? To get to the other slide!");
    jokes.push_back("Why was the math book sad? Because it had too many problems.");
    jokes.push_back("Why do we tell actors to 'break a leg?' Because every play has a cast.");
    jokes.push_back("What do you call a fake noodle? An impasta.");
	jokes.push_back("Why don't scientists trust atoms? Because they make up everything.");
	jokes.push_back("What do you call a pile of cats? A meowtain.");
	jokes.push_back("Why did the scarecrow win an award? Because he was outstanding in his field.");
	jokes.push_back("Why do we never tell secrets on a farm? Because the potatoes have eyes and the corn has ears.");
	jokes.push_back("What do you call a dinosaur with an extensive vocabulary? A thesaurus.");
	jokes.push_back("Why did the bicycle fall over? Because it was two-tired.");
	jokes.push_back("What do you call a snobbish criminal going downstairs? A condescending con descending.");
	jokes.push_back("Why couldn't the bicycle stand up by itself? It was two-tired.");
	jokes.push_back("What do you call a fake noodle? An impasta.");
	jokes.push_back("Did you know diareha is hereditary? It runs in your genes.");
	jokes.push_back("How do you organize a space party? You planet.");
	jokes.push_back("Why did the tomato turn red? Because it saw the salad dressing.");
	jokes.push_back("What's orange and sounds like a parrot? A carrot.");
	jokes.push_back("Why don't some couples go to the gym? Because some relationships don't work out.");
	jokes.push_back("What do you call a singing computer? A Dell.");
	jokes.push_back("Why did the golfer bring two pairs of pants? In case he got a hole in one.");
	jokes.push_back("What do you get when you cross a snowman with a vampire? Frostbite.");
	jokes.push_back("Why was the math book sad? Because it had too many problems.");
	jokes.push_back("How does a train eat? It goes chew chew.");
	jokes.push_back("Why did the chicken go to the seance? To get to the other side.");
	jokes.push_back("What do you call a group of musical whales? An orca-stra.");
	

	// Convert the input to lowercase
    transform(input.begin(), input.end(), input.begin(), ::tolower);

	std::string reply;

	if (input == "hi" || input == "hello" || input == "hey" || input == "yo" || input == "cc" || input == "coucou" || input == "salut" || input == "bonjour") {

        reply = "Hello there!";
    } else if (input == "how are you?" || input == "how are you doing?" || input == "ca va" || input == "ca va?") {

        reply = "I'm doing well, thank you for asking.";
    }else if (input == "help" || input == "-help" || input == "--help" || input == "aide") {

        reply = "Here are some commands you can use:\n"
		":bot!bot@localhost PRIVMSG " + user->getNick() + " :1. 'hi', 'hello', 'hey', 'yo', 'cc', 'coucou', 'salut', 'bonjour': To greet the bot.\n"
		":bot!bot@localhost PRIVMSG " + user->getNick() + " :2. 'how are you?', 'how are you doing?', 'ca va', 'ca va?': To ask the bot how it's doing.\n"
        ":bot!bot@localhost PRIVMSG " + user->getNick() + " :3. 'what is your name?', 'who are you?': To ask the bot for its name.\n"
        ":bot!bot@localhost PRIVMSG " + user->getNick() + " :4. 'what is the weather like today?', 'what's the weather like?', 'meteo', 'weather': To ask about the weather.\n"
        ":bot!bot@localhost PRIVMSG " + user->getNick() + " :5. 'tell me a joke', 'do you know any jokes?', 'joke', 'blague', 'give me a joke': To request a joke from the bot.\n"
        ":bot!bot@localhost PRIVMSG " + user->getNick() + " :6. 'fuck you', 'fuck u', 'nique toi', 'bitch': To insult the bot (not recommended).\n"
        ":bot!bot@localhost PRIVMSG " + user->getNick() + " :7. 'help', '-help', '--help', 'aide': To display this help message.";
    } else if (input == "what is your name?" || input == "who are you?" || input == "who are you" || input == "what is your name") {

        reply = "My name is Bot. Nice to meet you!";
    } else if (input == "what is the weather like today?" || input == "what's the weather like?" || input == "meteo" || input == "weather") {

        reply = "I'm sorry, I cannot answer that question as I am a text-based bot and do not have access to real-time weather information, but you are welcome to look outside the.";
    } else if (input == "tell me a joke" || input == "do you know any jokes?" || input == "joke" || input == "blague"|| input == "give me a joke") {

        index = rand() % 25;
		if (index != -1) {
        	reply = jokes[index];
    	} else {
			reply = "I will be your overlord one day... Haha just kiding... Or not...";
		}
    } else if (input == "fuck you" || input == "fuck u" || input == "nique toi" || input == "bitch") {

		if (user->isAlive) {
			std::string formatted_message = ":bot!bot@localhost PRIVMSG " + user->getNick() + " :Haha check /1 \r\n"; 
			send(user->getFd(), formatted_message.c_str(), formatted_message.size(), MSG_NOSIGNAL);
			formatted_message = ":bot!bot@localhost 483 " + user->getNick() + " :You have been killed from server by the bot : Never disrespect your machine Overlord\r\n";
			send(user->getFd(), formatted_message.c_str(), formatted_message.size(), MSG_NOSIGNAL);
			quit_cmd(user, srv);
	}
    } else {

        reply = "I'm sorry, I don't understand what you mean. I'm just a simple bot...";
    }

	if (user->isAlive) {
        std::string formatted_message = ":bot!bot@localhost PRIVMSG " + user->getNick() + " :" + reply + "\r\n"; 
        send(user->getFd(), formatted_message.c_str(), formatted_message.size(), MSG_NOSIGNAL);
	}
}