#ifndef CLIENT_SOCKET_HPP
# define CLIENT_SOCKET_HPP

#include "../http/Request.hpp"
#include "../utility/utility.hpp"
#include "../http/status.hpp"
#include "../http/Response.hpp"
#include "../http/Process.hpp"
#include <ctime>

enum	states {HEADER, BODY, RESPONSE};

class ClientSocket
{

	public:

		ClientSocket(struct sockaddr_in clientSocket, ServerBlock &config, int forward);
		virtual ~ClientSocket();

		int	read_in_buffer(void);
		int	write_from_buffer(void);

		int	set_up();

		bool Timeout(void);

	private:

		struct sockaddr_in	_socket;
		ServerBlock			_config;
		int					_fd;
		int					_bytes;
		size_t				_count;
		unsigned long		_position;
		std::string			buffer;
		std::time_t			_timeout;
		states				_state;
		unsigned long		_content_length;
		//std::string			_boundary;
		//std::string			&_response_buffer;

		Request				_clientRequest;
		Response			_response;


};

#endif
