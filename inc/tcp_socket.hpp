#ifndef TCP_SOCKET_HPP
# define TCP_SOCKET_HPP

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>


class tcp_socket
{
	public:

	
	private:
	int		_port;
	int		_server_fd;
	int		_new_socket;
	long	_valread;
	struct sockaddr_in address;
};

# endif