#include "../../inc/server.hpp"

Server::Server(std::string serv_config) : _config(serv_config)
{
	parse_serv();
}

Server::~Server(void)
{

}

bool	Server::parse_serv(void)					//parses the server configuration and initializes the related membervariables
{
	int	port = 8080;
	_tcp_socket = Tcp_socket(port);					//constructs the Tcp_socket object with the given port

	_server_name = "PetRoulette";
	_body_size = 100;

	std::string	loc_config;
	_locations.push_back(Location(loc_config));		//constructs the Location objects taking the configuration as parameter

	return (true);
}

void	Server::launch(void)
{
	_tcp_socket.launch();							//launches the Tcp_socket object
}