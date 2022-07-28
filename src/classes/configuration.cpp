#include "../../inc/configuration.hpp"

configuration::configuration(std::string path) : _path(path)
{
	parse_config();

}

configuration::~configuration(void)
{

}

bool	configuration::parse_config(void)
{
	std::string	serv_config;		//string of config for every single server
	_servers.push_back(server(serv_config));	//push every server in the vector with their config
	return (true);
}

void	configuration::launch(void)
{
	//std::vector<server>::iterator	it = _servers.begin();
	std::vector<server>::iterator	ite = _servers.end();

	for (std::vector<server>::iterator	it = _servers.begin(); it != ite; it++)
	{
		(*it).launch();
	}
}