#include "../../inc/server.hpp"

server::server(std::string serv_config) : _config(serv_config)
{
	parse_serv();
}

server::~server(void)
{

}

bool	server::parse_serv(void)
{
	int	port = 8080;
	_tcp_socket = tcp_socket(port);

	_server_name = "PetRoulette";
	_body_size = 100;

	std::string	loc_config;
	_locations.push_back(location(loc_config));

	return (true);
}

void	server::launch(void)
{
	_tcp_socket.launch();
}