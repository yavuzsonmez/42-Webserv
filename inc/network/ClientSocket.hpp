#ifndef CLIENT_SOCKET_HPP
# define CLIENT_SOCKET_HPP

#include "../utility/utility.hpp"

class ClientSocket
{

	public:

		ClientSocket(struct sockaddr_in clientSocket);
		virtual ~ClientSocket();

	private:

		struct sockaddr_in	_socket;
		std::string			buffer;

};

#endif