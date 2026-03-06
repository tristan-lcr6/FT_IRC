*This project has been created as part of the 42 curriculum by tlecuyer, jferrand.*

# ft_irc

## Description

**ft_irc** is a simplified implementation of an **Internet Relay Chat (IRC) server** written in **C++ (C++98 standard)**.
The goal of the project is to reproduce the core behavior of a real IRC server while respecting strict networking and system programming constraints.

The server communicates with standard IRC clients (such as **irssi**) using the IRC protocol over **TCP/IP**.
It supports multiple simultaneous clients using **non-blocking I/O** and a single **poll() loop** to manage all socket events.

Users can authenticate, join channels, exchange messages, and manage channels using operator commands.

Implemented features include:

* User authentication
* Nickname and username management
* Channel creation and management
* Private and channel messaging
* Channel operator privileges
* Channel modes
* A channel bot (bonus feature)

The objective of the project is to understand:

* TCP networking
* asynchronous I/O
* event-driven server architecture
* the IRC protocol basics

---

## Instructions

### Compilation

Compile the project using:

```bash
make
```

Available Makefile rules:

```bash
make        # compile the project
make clean  # remove object files
make fclean # remove object files and executable
make re     # rebuild everything
```

---

### Running the server

The server is executed as follows:

```bash
./ircserv <port> <password>
```

Example:

```bash
./ircserv 6667 mypassword
```

Parameters:

* **port** – the TCP port on which the server listens for incoming connections
* **password** – the connection password required by clients

---

### Connecting with an IRC client

Example using **irssi**:

```bash
irssi
```

Then connect to the server:

```
/connect 127.0.0.1 6667 mypassword
```

Example workflow:

```
/nick myNick
/join #test
/msg myFriend Hello!
```

---

## Implemented Commands

The server currently implements the following IRC commands:

### Authentication

| Command							| Description                         |
| ---------------------------------	| ----------------------------------- |
| `PASS <password>`					| Sets the connection password        |
| `NICK <nickname>`					| Sets or changes the user's nickname |
| `USER <username> 0 * :<realname>`	| Registers the user                  |

---

### Channel Management

| Command								| Description															|
| -------------------------------------	| ---------------------------------------------------------------------	|
| `JOIN <channel>`						| Join or create a channel												|
| `NAMES <channel>`						| List users in a channel												|
| `WHO <mask>`							| Display information about a single user or all the users of a channel	|
| `TOPIC <channel> [:newtopic]`			| View or change the channel topic										|
| `MODE <channel> [modes] [params]`		| Manage channel modes													|
| `INVITE <nickname> <channel>`			| Invite a user to a channel											|
| `KICK <channel> <client> [:reason]`	| Remove a user from a channel											|

---

### Messaging

| Command							| Description                       |
| ---------------------------------	| --------------------------------- |
| `PRIVMSG <msgtarget> :<message>`	| Send a private or channel message |

---

### Connection Management

| Command			| Description                |
| -----------------	| -------------------------- |
| `QUIT [:reason]`	| Disconnect from the server |

---

### Bonus

| Command			| Description                              |
| -----------------	| ---------------------------------------- |
| `BOT <channel>`	| Enable or disable a bot inside a channel |

Example:

```
BOT #channel
```

This command activates or deactivates a bot that can interact with users in the specified channel.

When someones says the word **"weather"** in that channel the bot says something about the weather, and if someone says a curse word (**fuck, asshole, bitch**) the bot roasts him.

---

## Channel Modes

The following channel modes are supported:

| Mode | Description                              |
| ---- | ---------------------------------------- |
| +i   | Invite-only channel                      |
| +t   | Only operators can change the topic      |
| +k   | Add or remove channel password           |
| +o   | Give or remove operator privileges       |
| +l   | Limit the number of users in the channel |

Example:

```
MODE #channel +k password
MODE #channel +l 10
MODE #channel +o user
MODE #channel -k+it
```

---

## Architecture

The server is structured around three main classes:

* `Server`
* `Client`
* `Channel`

These classes separate the responsibilities of **network management**, **user representation**, and **channel management**, following a modular architecture.

The server follows a centralized architecture where a single `Server` instance manages:

* all connected clients
* all active channels
* the event loop using `poll()`

Clients communicate with the server using the **IRC protocol over TCP/IP**.

The server parses incoming commands and dispatches them to the corresponding command handlers.


		Clients (IRC client)
			   |
			   v
		+-------------+
		|   Server    |
		|-------------|
		| poll() loop |
		| command     |
		| dispatcher  |
		+------+------+ 
			   |
		  +----+----+
		  |         |
		  v         v
		Client    Channel
		objects   objects


---

### Server

The `Server` class is the **core component** of the IRC server.
It manages the network layer, client connections, command dispatching, and channel storage.

#### Responsibilities

The `Server` class is responsible for:

* creating and configuring the **listening socket**
* accepting **new client connections**
* maintaining the **poll file descriptor list**
* handling incoming data from clients
* parsing and executing IRC commands
* managing the list of **clients** and **channels**
* broadcasting messages when required
* handling disconnections and cleanup

Key responsibilities:

	Server
	├── manage poll() loop
	├── accept new clients
	├── receive messages
	├── parse commands
	├── dispatch commands
	├── manage channels
	└── broadcast messages

#### Data structures

The server maintains several internal containers:

* `std::vector<Client*> _clients`
  Stores all currently connected clients.

* `std::vector<pollfd> _fds`
  Contains the file descriptors used by the `poll()` loop.

* `std::vector<Channel*> _channels`
  Stores all existing channels on the server.

#### Event loop

The server runs an **event-driven loop** based on `poll()`.

The loop performs the following steps:

1. Wait for activity on sockets.
2. Detect new incoming connections.
3. Accept new clients.
4. Receive data from existing clients.
5. Append received data to the client's buffer.
6. Extract complete IRC commands (`\r\n` terminated).
7. Dispatch the command to the appropriate handler.

Each IRC command is implemented as a dedicated method:

* `cmdJoin`
* `cmdPrivMsg`
* `cmdMode`
* `cmdKick`
* `cmdInvite`
* `cmdTopic`
* etc.

---

### Client

The `Client` class represents a **connected IRC user**.

Each instance stores the information associated with a single TCP connection.

#### Stored information

A `Client` object contains:

* the socket file descriptor (`_fd`)
* authentication state (`_authStatus`)
* nickname (`_nickName`)
* username (`_userName`)
* real name (`_realName`)
* IP address (`_IP`)
* a message buffer (`_buff`)

#### Authentication state

The authentication process follows the IRC registration sequence:

1. `PASS`
2. `NICK`
3. `USER`

The `_authStatus` field is used to track whether the client has completed the registration.

#### Buffer management

Since TCP is a stream protocol, messages may arrive in **partial packets**.

The client therefore maintains a buffer:

```
_buff
```

Incoming data is appended to the buffer.
The server extracts complete commands separated by `\r\n` before processing them.

#### Message sending

The method:

```
sendMessageOnClientFd()
```

is used to send formatted IRC responses directly to the client socket.

---

### Channel

The `Channel` class represents an **IRC channel** (for example `#general`).

It manages:

* channel members
* operators
* channel modes
* invitations
* channel topic
* message broadcasting

#### Client management

Each channel maintains a list of its members:

```
std::vector<Client*> _clients
```

Clients can:

* join a channel
* leave a channel
* be kicked by operators

The channel also maintains a list of operators:

```
std::vector<Client*> _operators
```

Operators have permission to execute privileged commands such as:

* `KICK`
* `MODE`
* `INVITE`
* `TOPIC` (depending on channel modes)

---

#### Channel modes

The channel implements several IRC modes:

| Mode | Description                         |
| ---- | ----------------------------------- |
| `+i` | Invite-only channel                 |
| `+t` | Only operators can modify the topic |
| `+k` | Channel password                    |
| `+o` | Operator privileges                 |
| `+l` | Limit number of users               |

The channel stores the state of these modes internally using boolean flags and additional parameters such as:

* `_password` for `+k`
* `_client_limit` for `+l`

---

#### Messaging

Channels implement broadcasting functions that send messages to all members:

```
sendChannelMessage()
```

Messages sent to a channel are automatically relayed to all connected clients within that channel.

---

#### Topic management

Channels store their topic and metadata:

* `_topic`
* `_topicwhotime`

This allows the server to send the appropriate IRC replies when users request the channel topic.

---

### Bot (bonus feature)

Each channel contains an instance of a `Bot`.

When activated, the bot can send automated messages to the channel using:

```
sendChannelMessageBot()
```

The bot behaves like a virtual user interacting with the channel.

---

### Design Overview

The architecture separates concerns as follows:

| Component   | Responsibility                             |
| ----------- | ------------------------------------------ |
| **Server**  | Networking, command dispatch, global state |
| **Client**  | Representation of a connected user         |
| **Channel** | Channel logic and member management        |

This separation simplifies the implementation and ensures that each class manages a well-defined part of the IRC protocol.

                    +-----------------------+
                    |       IRC CLIENT      |
                    |  (irssi / netcat)     |
                    +-----------+-----------+
                                |
                                | TCP/IP
                                |
                                v
                    +-----------------------+
                    |        Server         |
                    |-----------------------|
                    | - _port               |
                    | - _password           |
                    | - _clients            |
                    | - _channels           |
                    | - _fds (poll list)    |
                    +-----------+-----------+
                                |
                +---------------+---------------+
                |                               |
                v                               v
        +---------------+               +---------------+
        |    Client     |               |    Channel    |
        |---------------|               |---------------|
        | _fd           |               | _name         |
        | _nickName     |               | _topic        |
        | _userName     |               | _clients      |
        | _realName     |               | _operators    |
        | _IP           |               | _invite_list  |
        | _buffer       |               | _modes        |
        +---------------+               +---------------+
                |                               |
                |                               |
                +----------- users -------------+
                             join

### Event loop (poll)

The server uses a **single poll() loop** to manage all network events.

This allows the server to support multiple clients simultaneously **without using threads or forks**.

Main loop:

```
while (server_running)
{
    poll(_fds)

    if (new connection)
        accept client

    if (client sends data)
        recv()

    parse commands

    execute command
}
```

The following diagram illustrates the poll() loop.

					+---------------------+
					|  Server Start       |
					+----------+----------+
							   |
							   v
					+---------------------+
					|  Create Socket      |
					|  bind() / listen()  |
					+----------+----------+
							   |
							   v
					+---------------------+
					|  Initialize poll()  |
					|  add server socket  |
					+----------+----------+
							   |
							   v
						+-------------+
						|   poll()    |
						+------+------+ 
							   |
			+------------------+--------------------+
			|                                       |
			v                                       v
	+---------------+                     +------------------+
	| New Connection|                     | Client Activity  |
	| on server fd  |                     | on client fd     |
	+-------+-------+                     +---------+--------+
			|                                       |
			v                                       v
	+---------------+                     +------------------+
	| accept()      |                     | recv()           |
	| create Client |                     | append to buffer |
	| add to poll() |                     +---------+--------+
	+-------+-------+                               |
			|                                       v
			|                          +--------------------------+
			|                          | Extract complete command |
			|                          | (split by \r\n)          |
			|                          +------------+-------------+
			|                                       |
			|                                       v
			|                          +--------------------------+
			|                          | execute(command)         |
			|                          | dispatch IRC command     |
			|                          +------------+-------------+
			|                                       |
			|                                       v
			|                          +--------------------------+
			|                          | send responses           |
			|                          | broadcast if needed      |
			|                          +------------+-------------+
			|                                       |
			+------------------------<--------------+
							back to poll()

### Message parsing

Since TCP does not guarantee message boundaries, incoming data may arrive in **multiple fragments**.

Each client maintains a **buffer** where incoming data is stored.

Example:

	recv() → "JO"
	recv() → "IN #te"
	recv() → "st\r\n"

The server reconstructs the full command:

	JOIN #test

Commands are separated using the IRC delimiter:

	\r\n

Only when a full command is reconstructed it is passed to the command dispatcher.

---

## Technical Constraints

This project follows strict constraints:

* **C++98 standard**
* **Single poll() loop**
* **Non-blocking sockets**
* **No multithreading**
* **No forking**
* **TCP/IP communication**

Allowed system calls include:

```
socket
bind
listen
accept
send
recv
poll
fcntl
close
```

---

## Testing

The server can be tested using:

### IRC clients

* irssi
* nc (netcat)

Example with netcat:

```bash
nc -C 127.0.0.1 6667
```

Example test:

```
PASS mypassword
NICK user
USER user 0 * :Real Name
JOIN #test
```

---

## Resources

The following resources were useful during the development of this project:

### IRC protocol

* RFC 1459 – Internet Relay Chat Protocol
  https://datatracker.ietf.org/doc/html/rfc1459

* RFC 2812 – IRC Client Protocol
  https://datatracker.ietf.org/doc/html/rfc2812

* IRC Numerics
  https://gist.github.com/zorael/4ecdc3250d0ba72c3755f970fcabbff4

### Networking

* Beej's Guide to Network Programming
  https://beej.us/guide/bgnet/

* Linux socket documentation
  https://man7.org/linux/man-pages/

* IRC server guide
  https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9

---

## AI Usage

AI tools were used during this project to assist with:

* understanding the IRC protocol
* clarifying numeric reply formats
* debugging networking issues
* improving documentation

AI was **not used to generate the full implementation** of the server.
All architectural and coding decisions were made by the project authors.

---

## Authors

* tlecuyer
* jferrand

