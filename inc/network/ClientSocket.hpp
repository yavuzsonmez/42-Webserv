#ifndef CLIENT_SOCKET_HPP
# define CLIENT_SOCKET_HPP

#include "../http/Request.hpp"
#include "../utility/utility.hpp"
#include "../http/status.hpp"
#include "../http/Response.hpp"
#include "../http/Process.hpp"
#include <poll.h>
#include <ctime>

enum	states {HEADER, BODY, RESPONSE, PIPE};

class ClientSocket
{
	public:

		ClientSocket(struct sockaddr_in clientSocket, ServerBlock &config, int forward);
		virtual ~ClientSocket();

		void	call_func_ptr(void);

		void	read_in_buffer(void);
		void	write_from_buffer(void);

		void	one(void);
		void	two(void);
		void	three(void);

		void	process_request(void);
		void	get_request(void);
		void	build_response(std::string path, std::string code, std::string status);
		void	build_dl_response(void);
		bool	check_location(void);
		std::string	get_location(std::string location, ConfigurationKeyType type);
		bool	get_location_dl(std::string location);
		void	exception(int e);
		int		get_fd_cgi(void);

		void	set_up();

		bool Timeout(void);
		
		short				_event;
		int					_fd;
		int					_client_fd;
		bool				_remove;

	private:

		struct sockaddr_in	_socket;
		ServerBlock			_config;
		int					_fd_cgi;
		int					_bytes;
		size_t				_count;
		unsigned long		_position;
		std::string			buffer;
		std::time_t			_timeout;
		states				_state;
		unsigned long		_content_length;
		void					(ClientSocket::*_func_ptr)(void);
		Request				_clientRequest;
		Process				_process;
};

#endif
