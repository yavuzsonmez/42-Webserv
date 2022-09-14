#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "../configuration_key/ConfigurationKey.hpp"
# include "../configuration_key/ServerBlock.hpp"

class Location
{
	public:
	Location(void);
	Location(ServerBlock config);
	Location(ConfigurationKey key);
	~Location(void);

	private:
	std::string	_index;
	std::string	_root;
};

# endif