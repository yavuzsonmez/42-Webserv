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
	//typedef			int (ClientSocket::*_func_ptr)(void);
	public:

		ClientSocket(struct sockaddr_in clientSocket, ServerBlock &config, int forward);
		virtual ~ClientSocket();

		int	call_func_ptr(void);

		int	read_in_buffer(void);
		int	write_from_buffer(void);

		int	one(void);
		int	two(void);
		int	three(void);

		void	process_request(void);
		void	get_request(void);
		void	build_response(std::string path, std::string code, std::string status);
		void	build_dl_response(void);
		bool	check_location(void);
		std::string	get_location(std::string location, ConfigurationKeyType type);
		bool	get_location_dl(std::string location);
		void	exception(int e);
		int		get_fd_cgi(void);

		int	set_up();

		bool Timeout(void);
		short				_event;
		int					_fd;
		int					_client_fd;
		bool				_remove;

	private:

		struct sockaddr_in	_socket;
		ServerBlock			_config;
		
		int					_fd_cgi;
		// int					_pipefd_in[2];
		// int					_pipefd_out[2];
		int					_bytes;
		size_t				_count;
		unsigned long		_position;
		std::string			buffer;
		std::time_t			_timeout;
		states				_state;
		unsigned long		_content_length;

		//_func_ptr			a;
		int					(ClientSocket::*_func_ptr)(void);
		//std::string			_boundary;
		//std::string			&_response_buffer;

		Request				_clientRequest;
		//Response			_response;
		Process				_process;
		// CGI					_CGI;
		

		// std::vector<method>	_methods;
		// std::string	_redirection;
		// std::string	_cgi;
		// std::string	_cgi_fileending;


};

#endif
