#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <string>

class	Location
{
	public:
	Location(std::string loc_config);
	~Location(void);

	bool	parse_loc(void);

	private:
	std::string	_config;
	std::string	_root;
	std::string	_index;
};

# endif