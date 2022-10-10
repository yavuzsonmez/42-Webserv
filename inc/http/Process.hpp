#ifndef PROCESS_HPP
# define PROCESS_HPP

# include "Response.hpp"
# include "Request.hpp"
# include "Cgi.hpp"
# include "../configuration_key/ConfigurationKey.hpp"
# include <cstdio>

class Process
{
	public:
	Process(Response &response, Request request, ServerBlock &config);
	~Process(void);

	void	process_request(void);
	void	get_request(void);
	void	post_request(void);
	void	delete_request(void);
	void	build_response(std::string path, std::string code, std::string status);
	bool	check_location(void);
	std::string	get_location(std::string path, ConfigurationKeyType type);
	void	exception(int code);

	

	private:
	Response	&_response;
	Request		_request;
	ServerBlock	&_config;
	std::string	_cgi;
	std::string	_cgi_fileending;
	std::string	_redirection;
	std::vector<method>	_methods;


};


# endif