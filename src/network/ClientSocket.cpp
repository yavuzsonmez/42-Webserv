#include "../../inc/network/ClientSocket.hpp"

ClientSocket::ClientSocket(struct sockaddr_in clientSocket)
{
	//if (!_socket)
	// fail exit

	_socket.sin_family = clientSocket.sin_family;
	_socket.sin_port = clientSocket.sin_port;
	_socket.sin_addr.s_addr = clientSocket.sin_addr.s_addr;
	bzero(&(_socket.sin_zero), 8);

}

ClientSocket::~ClientSocket()
{

}