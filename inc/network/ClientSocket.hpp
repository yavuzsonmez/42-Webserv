#ifndef CLIENT_SOCKET_HPP
# define CLIENT_SOCKET_HPP

# include <iostream>
# include <string>

# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <fcntl.h>

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
