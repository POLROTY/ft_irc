# My IRC Server

## Introduction
this IRC Server is a simple and lightweight IRC server written in C++98. It is designed to be easy to set up and use.

## Features
- Supports all basic IRC commands, such as JOIN, PART, PRIVMSG, and more
- Easy to set up and configure
- Lightweight and efficient
- Made for irssi but compatible with netcat

## Installation
To install this IRC Server, follow these steps:

1. Clone the repository: `git clone https://github.com/POLROTY/ft_irc.git`
2. Build the server: `make`
3. Start the server: `./ircserv portNumber password`

## Usage
To use this IRC Server, you will need an IRC client, such as irssi. Here are the steps to connect:

1. Open your IRC client and enter the following command: `/connect localhost 6667 password`
2. Once you are connected, you can join channels using the JOIN command, send messages using the PRIVMSG command, and more.
3. For the bot usage: `/msg bot message`
3. To send a file to an irssi client : `/dcc send <nickname> <path><filename>`
4. To accept a file transfer from an irssi client: `/dcc get <nickname> <filename>`

## Commands

- `/nick`: Change your nickname.
- `/msg`: Send a message to a user, channel or bot.
- `/join`: Join a channel.
- `/invite`: Invite a user to a channel. (+i mode)
- `/topic`: Change the topic of a channel. (need +o status)
- `/kick`: Kick a user from a channel. (need +o status)
- `/part`: Leave a channel.
- `/mode`: Change the mode of a channel or user. (iob)
- `/whois`: Get information about a user. (if user is -i)
- `/who`: Get a list of users on a channel. (if users are -i)
- `/oper`: Authenticate as an IRC operator.
- `/notice`: Send a notification to all users. (need +o status)
- `/kill`: Kill a user from the server or the server itself. (need +o status)
- `/quit`: Disconnect from the server.

For more information about each command, see the IRC specification at https://tools.ietf.org/html/rfc1459.

## Examples
Here are some examples of basic IRC commands:

- Join a channel: `/join mychannel` in irssi, `JOIN #mychannel` in netcat
- Send a message to a user: `/msg username Hello, how are you?` in irssi, `PRIVMSG username :Hello, how are you?` in netcat
- Send a message to a channel: `/msg #mychannel Hello, everyone!` in irssi, `PRIVMSG #mychannel :Hello, how are you?` in netcat
- Quit the server: `/quit` in irssi, `QUIT ` in netcat
- Send a message to the bot: `/msg bot message` in irssi, `PRIVMSG bot :message` in netcat
- Send a file to an irssi client: `/dcc send <nickname> <path><filename>`
- Accept a file transfer from an irssi client: `/dcc get <nickname> <filename>`


## Contributing
If you would like to contribute to this IRC Server, feel free to submit a pull request.

## License
This project is licensed under the MIT License.