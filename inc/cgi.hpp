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

class	CGI
{
	public:
	CGI();
	~CGI();

	void	execute(void);
	std::string	get_buf(void);

	private:
	char	*env[17];
	std::string	_buf;
	FILE	*_tmp;
	int		_fd;
	int		_tmp_size;
	
};

# endif