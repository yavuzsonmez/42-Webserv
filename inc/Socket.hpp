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
		~Socket();

	private:

};

#endif
