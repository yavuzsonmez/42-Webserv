#include "../../inc/network/ClientSocket.hpp"



ClientSocket::ClientSocket(struct sockaddr_in clientSocket, int forward)
{
	//if (!_socket)
	// fail exit

	_socket.sin_family = clientSocket.sin_family;
	_socket.sin_port = clientSocket.sin_port;
	_socket.sin_addr.s_addr = clientSocket.sin_addr.s_addr;
	bzero(&(_socket.sin_zero), 8);

	_fd = forward;
	_position = 0;
	_count = 32000;
}

ClientSocket::~ClientSocket()
{

}

bool ClientSocket::Timeout()
{
	if (std::time(NULL) - _timeout > 5)
		return true;
	return false;

}


int	ClientSocket::read_in_buffer(void)
{
	buffer.resize(buffer.size() + _count);
	_bytes = read(_fd, (char*)buffer.data() + _position, _count);
	size_t pos = buffer.find("\r\n\r\n");
	if (pos != std::string::npos)
	{
		std::string httpRequestHead = buffer.substr(0, pos + 3);
		_clientRequest.parser(httpRequestHead);


	}

	//if (_bytes == 0 && Timeout())
	//	throw (Request_Timeout);
	//else if (_bytes == -1)
	//	throw (Internal_Server_Error);


	return 1;

}