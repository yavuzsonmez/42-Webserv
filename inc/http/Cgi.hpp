#ifndef CGI_HPP
# define CGI_HPP

# include <unistd.h>
# include <string>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <iostream>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <map>
# include <vector>
# include "../utility/utility.hpp"
# include "../configuration_key/ServerBlock.hpp"
# include <algorithm>
# include "Request.hpp"

/**
 * @brief class that gets instantiated whenever a cgiscript is called
 */

class	CGI
{
	public:
	CGI();
	CGI(Request request, ServerBlock &config);
	~CGI();

	void	execute(void);
	std::string	get_buf(void);
	std::string	get_query(std::string referer);

	private:
	std::map<std::string, std::string>	_env;
	std::vector<std::string>	_query_parameters;
	std::vector<std::string>	_argv;
	std::string	_buf;
	FILE	*_tmpout;
	FILE	*_tmpin;
	//int		_fd;
	int		_tmp_size;

	Request	_request;
	ServerBlock	_config;
};

# endif