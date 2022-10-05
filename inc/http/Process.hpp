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
	void	create_index(void);
	void	build_response(std::string path);
	bool	check_location(void);
	//ConfigurationKey	get_location(std::string path);
	std::string	get_location(std::string path, ConfigurationKeyType type);
	//int	get_location(std::string path);

	

	private:
	Response	&_response;
	Request		_request;
	ServerBlock	&_config;
	std::string	_cgi;
	std::string	_cgi_fileending;


};


# endif