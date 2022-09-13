#ifndef SERVER_SOCKET_HPP
# define SERVER_SOCKET_HPP

#include "../utility/utility.hpp"
#include "../configuration_key/ServerBlock.hpp"

#define BACKLOG 10 // maximum number of allowed incoming connection in the queue until being accept()

/**
 * @brief Server Socket listening for requests
 */
class ServerSocket
{

	public:

		ServerSocket(ServerBlock config, unsigned int address);

		//ServerSocket( const ServerSocket &src );
		virtual ~ServerSocket();

		int getFileDescriptor() const;

		void processConnections();

		// gets thrown if socket creation is faulty.
		class SocketCreationError : public std::exception {
			public:
				virtual const char* what() const throw() {
					return "socket couldn't be initialised properly, check errno.";
				}
		};

	private:

		int _fd;
		struct sockaddr_in	_socket;
		ServerBlock	_config;
};

#endif
