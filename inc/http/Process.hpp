#ifndef PROCESS_HPP
# define PROCESS_HPP

# include "Response.hpp"
# include "Request.hpp"

class Process
{
	public:
	Process(Response &response, Request request, ServerBlock config);
	~Process(void);

	void	process_request(void);
	void	create_index(void);
	bool	check_location(void);
	

	private:
	Response	&_response;
	Request		_request;
	ServerBlock	_config;


};


# endif