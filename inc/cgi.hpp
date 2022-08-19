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
# include "utility.hpp"
# include <algorithm>

class	CGI
{
	public:
	CGI();
	CGI(std::string request);
	~CGI();

	void	execute(void);
	std::string	get_buf(void);
	std::string	get_query(std::string referer);
	char	**create_envp(void);

	private:
	std::map<std::string, std::string>	_env;
	std::string	_buf;
	FILE	*_tmpout;
	FILE	*_tmpin;
	int		_fd;
	int		_tmp_size;
	

	std::string	_referer;		//for testing, must be part of the request class
};

# endif