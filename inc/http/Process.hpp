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
	Process();
	Process(/*Response &response, */Request request, ServerBlock config);
	~Process(void);
	Process &operator=(const Process &src);

	void	process_request(void);
	void	handle_request(void);
	void	post_request(void);
	void	delete_request(void);
	void	build_response(std::string path, std::string code, std::string status);
	void	build_dl_response(void);
	void	server_overloaded(void);
	void	build_cgi_response(void);
	bool	check_location(void);
	void	set_redirection_response(void);
	bool	detectCgi(std::string path, std::string code, std::string status);
	std::string	get_location(std::string location, ConfigurationKeyType type);
	bool	get_location_dl(std::string path);
	void	exception(int code);
	bool	check_if_request_is_too_large();

	// path builder
	std::string build_path_with_index__or_script_file();
	std::string getPathForNestedLocation();

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