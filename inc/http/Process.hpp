#ifndef PROCESS_HPP
# define PROCESS_HPP

# include "Response.hpp"
# include "Request.hpp"
# include "Cgi.hpp"
# include "../configuration_key/ConfigurationKey.hpp"
# include <cstdio>
# include <filesystem>

class Process
{
	public:
	Process();
	Process(/*Response &response, */Request request, ServerBlock config);
	~Process(void);
	Process &operator=(const Process &src);

	void	process_request(void);
	void	get_request(void);
	void	post_request(void);
	void	delete_request(void);
	void	build_response(std::string path, std::string code, std::string status);
	void	build_dl_response(void);
	void	build_cgi_response(void);
	bool	check_location(void);
	std::string	get_location(std::string location, ConfigurationKeyType type);
	bool	get_location_dl(std::string path);
	void	exception(int code);

	Response	_response;
	CGI			_CGI;
	bool		_with_cgi;

	private:
	//Response	_response;
	Request		_request;
	ServerBlock	_config;
	std::string	_cgi;
	std::string	_cgi_fileending;
	std::string	_redirection;
	std::vector<method>	_methods;
	int			_pipefd_in[2];
	int			_pipefd_out[2];

};


# endif