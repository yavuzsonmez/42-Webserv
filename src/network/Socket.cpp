#include "../../inc/Socket.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>

#define BACKLOG 10 // maximum number of allowed incoming connection in the queue until being accept()

int main(int argc, char const *argv[])
{
	int server_fd, forward;
	struct sockaddr_in server, client;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	// if socket return -1 throw error + perror
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(80);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(my_addr.sin_zero), 8);

	bind(server_fd, (struct sockaddr *)&server, sizeof(struct sockaddr));
	// if bind return -1 throw error + perror

	listen(server_fd, BACKLOG);
	// if bind return -1 throw error + perror

	forward = accept(server_fd, &client, sizeof(struct sockaddr_in));
	// if accept return -1 throw error

	char *response = "Hello World!\n"; //exemple of sending comething with error checking
	size_t len, bytes_send;
	len = strlen(response);
	bytes_send = 0;
	// if bytes_send < len we have to handle it and keep send until everything was sent
	while (bytes_send < len)
	{
		if (bytes_send == -1)
			// if send return -1, throw error; + perror
		bytes_send = send(forward, response + bytes_send, len - bytes_send, 0);
	}

	//if close(server) //throw error; + perror

	return 0;
}
