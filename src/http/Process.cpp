# include	"../../inc/http/Process.hpp"

Process::Process(Response &response, Request request, ServerBlock config) : _response(response), _request(request), _config(config)
{

}

Process::~Process(void)
{

}

void	Process::process_request(void)
{
	// std::cout << "script: " << _request.getScript().first << std::endl;
	// std::cout << "path: " << _request.getPath().first << std::endl;
	if (_request.getPath().first == "/")
	{
		create_index();
		return ;
	}
	else if (check_location())
	{
		std::cout << "test" << std::endl;
	}
	
	
	//if (check_location())

}

void	Process::create_index(void)
{
	_response.set_protocol("HTTP/1.1");
	_response.set_status_code("200");
	_response.set_status_text("OK");

	_response.set_server("PetRoulette");
	_response.set_content_type("text/html");
	_response.set_body(get_file_content(_config.getConfigurationKeysWithType(INDEX).front().indexes.front()));
	_response.set_content_length(to_str(_response.get_body().length()));
	_response.create_response();
}


bool	Process::check_location(void)
{
	std::vector<ConfigurationKey>	locations = _config.getConfigurationKeysWithType(LOCATION);
	std::vector<ConfigurationKey>::iterator	it;
	for (it = locations.begin(); it != locations.end(); it++)
	{
		std::string	request_path = _request.getPath().first.insert(0, "/");
		std::cout << "location: " << (*it).value.length() << std::endl;
		std::cout << "path: " << request_path.length() << std::endl;
		std::cout << "check: " << ((*it).location == request_path) << std::endl;
		if ((*it).location == request_path)
		{
			std::cout << "test1" << std::endl;
			return true;
		}
	}
	return false;
}

