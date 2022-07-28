#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include <string>

class	configuration
{
	public:
	configuration(std::string path);
	~configuration();

	private:
	std::vector<server>	_servers;

};

# endif