#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>

# include <sys/select.h>
# include <poll.h>
# include <sys/epoll.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <fcntl.h>

class Socket
{

	public:

		Socket();
		Socket( const Socket &src );
		virtual ~Socket();


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
		struct sockaddr_in	_socket


};

#endif
