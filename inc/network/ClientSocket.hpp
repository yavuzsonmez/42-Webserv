#ifndef CLIENT_SOCKET_HPP
# define CLIENT_SOCKET_HPP

#include "../utility/utility.hpp"

class ClientSocket
{

	public:

		ClientSocket(struct sockaddr_in clientSocket);
		virtual ~ClientSocket();

		struct sockaddr_in	* getSocket();

	private:

		struct sockaddr_in	* _socket;


};

#endif
