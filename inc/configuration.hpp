#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include <string>
# include "server.hpp"

class	configuration
{
	public:
	configuration(std::string path);
	~configuration(void);

	bool	parse_config(void);

	void	launch(void);

	private:
	std::string	_path;
	std::vector<server>	_servers;

};

# endif