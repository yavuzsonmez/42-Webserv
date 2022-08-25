#ifndef TCP_SOCKET_HPP
# define TCP_SOCKET_HPP

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include "response.hpp"


class Tcp_socket
{
	public:
	Tcp_socket(void);
	Tcp_socket(int port);
	~Tcp_socket(void);

	void	launch(void);

	private:
	int		_port;
	int		_server_fd;
	int		_new_socket;
	long	_valread;
	struct sockaddr_in _address;
	int		_addrlen;

	Response	_response;
};

# endif