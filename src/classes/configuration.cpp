#include "../../inc/configuration.hpp"

Configuration::Configuration(std::string path) : _path(path)
{
	parse_config();

}

Configuration::~Configuration(void)
{

}

bool	Configuration::parse_config(void)
{
	std::string	serv_config;					//string of config for every single server
	_servers.push_back(Server(serv_config));	//constructs a Server object, taking their config as a parameter and pushes it in the vector
	return (true);
}

void	Configuration::launch(void)
{
	//std::vector<server>::iterator	it = _servers.begin();
	std::vector<Server>::iterator	ite = _servers.end();

	for (std::vector<Server>::iterator	it = _servers.begin(); it != ite; it++)	//launches every constructed Server object in the vector
	{
		(*it).launch();
	}
}