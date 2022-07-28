#include "../../inc/tcp_socket.hpp"

tcp_socket::tcp_socket(void)
{

}

tcp_socket::tcp_socket(int port) : _port(port)
{

}

tcp_socket::~tcp_socket(void)
{

}

void	tcp_socket::launch(void)
{
	_addrlen = sizeof(_address);

	std::string	hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

	// Creating socket file descriptor
	if ((_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("In socket");
		exit(EXIT_FAILURE);
	}


	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons( _port );

	memset(_address.sin_zero, '\0', sizeof _address.sin_zero);


	if (bind(_server_fd, (struct sockaddr *)&_address, sizeof(_address))<0)
	{
		perror("In bind");
		exit(EXIT_FAILURE);
	}
	if (listen(_server_fd, 10) < 0)
	{
		perror("In listen");
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		if ((_new_socket = accept(_server_fd, (struct sockaddr *)&_address, (socklen_t*)&_addrlen))<0)
		{
			perror("In accept");
			exit(EXIT_FAILURE);
		}

		char buffer[30000] = {0};
		_valread = read( _new_socket , buffer, 30000);
		printf("%s\n",buffer );
		write(_new_socket , hello.c_str() , hello.length());
		printf("------------------Hello message sent-------------------");
		close(_new_socket);
	}
	return ;
}