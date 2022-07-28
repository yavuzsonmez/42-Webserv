#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <vector>
# include "tcp_socket.hpp"
# include "location.hpp"

class server
{
	public:
	server(std::string serv_config);
	~server(void);

	bool	parse_serv(void);

	void	launch(void);

	private:
	std::string				_config;
	tcp_socket				_tcp_socket;
	std::string				_server_name;
	int						_body_size;
	std::vector<location>	_locations;
};

# endif