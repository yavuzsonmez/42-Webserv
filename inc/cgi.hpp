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

class	CGI
{
	public:
	CGI();
	~CGI();

	void	execute(void);
	std::string	get_buf(void);
	char	**create_envp(void);

	private:
	std::map<std::string, std::string>	_env;
	std::string	_buf;
	FILE	*_tmp;
	int		_fd;
	int		_tmp_size;
	
};

# endif