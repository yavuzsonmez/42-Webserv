#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include <string>
# include "Server.hpp"

class	Configuration
{
	public:
	Configuration(std::string path);
	~Configuration(void);

	bool	parse_config(void);			//parses the configfile and constructs Server objects with their related configuration as parameter

	void	launch(void);				//launches every object in _servers

	private:
	std::string	_path;					//path to the configfile
	std::vector<Server>	_servers;		//vector to store all Server objects

};

# endif