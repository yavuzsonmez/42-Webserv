#ifndef CLIENT_SOCKET_HPP
# define CLIENT_SOCKET_HPP

#include "../http/Request.hpp"
#include "../utility/utility.hpp"
#include "../http/status.hpp"
#include <ctime>

class ClientSocket
{

	public:

		ClientSocket(struct sockaddr_in clientSocket, int forward);
		virtual ~ClientSocket();

		int	read_in_buffer(void);

		bool Timeout(void);

	private:

		struct sockaddr_in	_socket;
		int					_fd;
		int					_bytes;
		size_t				_count;
		int					_position;
		std::string			buffer;
		std::time_t			_timeout;

		Request				_clientRequest;


};

#endif
