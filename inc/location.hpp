#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <string>

class	location
{
	public:
	location(std::string loc_config);
	~location(void);

	bool	parse_loc(void);

	private:
	std::string	_config;
	std::string	_root;
	std::string	_index;
};

# endif