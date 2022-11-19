#ifndef CGI_HPP
# define CGI_HPP

# include <unistd.h>
# include <string>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
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

extern char **environ;

/**
 * @brief class that gets instantiated whenever a cgiscript is called
 */

class	CGI
{
	public:
	CGI();
	CGI(Request request, ServerBlock config, std::string path, std::string cgi_path);
	~CGI();
	CGI	&operator=(const CGI &src);

	//void	execute(void);
	void		set_tmps(void);
	void		write_in_std_in(void);
	void		set_environment(Request request, ServerBlock config, std::string path, std::string cgi_path);
	void		write_in_std_out(void);
	void		read_in_buff(void);
	static void	sig_handler(int signum);
	std::string	get_buf(void);

	std::string	get_query(std::string referer);

	//int			_pipefd_in[2];
	//int			_pipefd_out[2];
	int			_fd_in;
	int			_fd_out;

	private:
	std::map<std::string, std::string>	_env;
	char	**_envp;
	std::vector<std::string>	_query_parameters;
	char	**_argvp;
	std::vector<std::string>	_argv;
	std::string	_buf;
	FILE	*_tmpout;
	FILE	*_tmpin;
	//int		_fd;
	int		_tmp_size;

	Request	_request;
	ServerBlock	_config;
	std::string	_path;
	std::string	_cgi_path;
};

# endif