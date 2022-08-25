#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <vector>
# include "tcp_socket.hpp"
# include "location.hpp"

class Server
{
	public:
	Server(std::string serv_config);
	~Server(void);

	bool	parse_serv(void);				//parses the configuration

	void	launch(void);

	private:
	std::string				_config;		//string of the configuration
	Tcp_socket				_tcp_socket;	//Tcp_socket object
	std::string				_server_name;
	int						_body_size;
	std::vector<Location>	_locations;		//vector of all given Location objects
};

# endif