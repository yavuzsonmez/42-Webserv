#ifndef SERVER_SOCKET_HPP
# define SERVER_SOCKET_HPP

#include <poll.h>
#include <cerrno>
#include "../utility/utility.hpp"
#include "../configuration_key/ServerBlock.hpp"
#include "./ClientSocket.hpp"

#define BACKLOG 10 // maximum number of allowed incoming connection in the queue until being accept()

/**
 * @brief Server Socket listening for requests
 */
typedef std::vector<std::pair<int, ClientSocket> >::iterator	client_iter;

class ServerSocket
{

	public:

		ServerSocket(ServerBlock config, unsigned int address);

		//ServerSocket( const ServerSocket &src );
		virtual ~ServerSocket();

		//int getFileDescriptor() const;

		void processConnections();

		client_iter	get_CS_position(std::vector<std::pair<int, ClientSocket> > &vector, int key);

		// gets thrown if socket creation is faulty.
		class SocketCreationError : public std::exception {
			public:
				virtual const char* what() const throw() {
					return "socket couldn't be initialised properly, check errno.";
				}
		};

	private:
		std::vector<int>	_fds;
		std::vector<struct sockaddr_in>	_sockets;
		//std::map<int, ClientSocket> _clients;
		//std::map<unsigned long, ClientSocket> _clients;
		std::vector<std::pair<int, ClientSocket> >	_clients;
		ServerBlock	_config;
		unsigned int listeningSockets;
};

#endif
