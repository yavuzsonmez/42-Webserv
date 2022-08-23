#ifndef SERVER_SOCKET_HPP
# define SERVER_SOCKET_HPP

# include <iostream>
# include <string>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <cstring>

# include <sys/select.h>
# include <poll.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <fcntl.h>

#define BACKLOG 10 // maximum number of allowed incoming connection in the queue until being accept()

/**
 * @brief Server Socket listening for requests
 */
class ServerSocket
{

	public:

		ServerSocket(unsigned short port, unsigned int address);
		ServerSocket( const ServerSocket &src );
		virtual ~ServerSocket();

		const int getFileDescriptor() const;

		void acceptConnection();

		// gets thrown if socket creation is faulty.
		class SocketCreationError : public std::exception {
			public:
				virtual const char* what() const throw() {
					return "socket couldn't be initialised properly, check errno.";
				}
		};

	private:

		int _fd;
		struct sockaddr_in	_socket;


};

#endif
