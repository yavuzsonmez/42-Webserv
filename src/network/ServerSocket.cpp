#include "../../inc/network/ServerSocket.hpp"
#include "../../inc/network/ClientSocket.hpp"

ServerSocket::ServerSocket(unsigned short port, unsigned int address)
{
	_fd = socket(AF_INET, SOCK_STREAM, 0); //IPv4, TCP
	if (_fd < 0)
		throw SocketCreationError();

	_socket.sin_family = AF_INET;
	_socket.sin_port = htons(port); //host byte order to network byte order
	_socket.sin_addr.s_addr = address;
	bzero(&(_socket.sin_zero), 8);

	if (bind(_fd, (struct sockaddr *)&_socket, sizeof(struct sockaddr_in)))
		throw SocketCreationError();

	if (listen(_fd, BACKLOG))
		throw SocketCreationError();
}

const int ServerSocket::getFileDescriptor() const { return _fd; }

int main(int argc, char const *argv[])
{
	int forward;
	struct sockaddr_in clientSocket;
	socklen_t socketSize = sizeof(struct sockaddr_in);
	ServerSocket server(4242, INADDR_ANY); // testing


	forward = accept(server.getFileDescriptor(), (struct sockaddr *)&clientSocket, &socketSize);
	// if accept return -1 throw error
	ClientSocket client(clientSocket);

	char *response = "Hello World!\n"; //exemple of sending something with error checking
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
