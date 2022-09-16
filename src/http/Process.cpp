# include	"../../inc/http/Process.hpp"

Process::Process(Response &response, Request request, ServerBlock &config) : _response(response), _request(request), _config(config)
{

}

Process::~Process(void)
{

}

void	Process::process_request(void)
{
	if (_request.getPath().first == "/")
	{
		std::string path = _config.getConfigurationKeysWithType(ROOT).front().root + "/" + _config.getConfigurationKeysWithType(INDEX).front().indexes.front();
		std::cout << "index-path: " << path << std::endl;
		build_response(path);
		return ;
	}
	else if (check_location())
	{
		if (_request.getScript().first.empty())
		{
			std::cout << "request: " << _request.getPath().first << std::endl;
			//std::string path = get_location(_request.getPath().first.insert(0, "/")).root + "/" + get_location(_request.getPath().first.insert(0, "/")).indexes.front();
			std::string path = get_location(_request.getPath().first.insert(0, "/"), ROOT) + "/" + get_location(_request.getPath().first.insert(0, "/"), INDEX);
			std::cout << "without-script-path: " << path << std::endl;
			build_response(path);
		}
		else
		{
			
			std::string path = get_location(_request.getPath().first.insert(0, "/"), ROOT) + "/" + _request.getScript().first;
			std::cout << "with-scriptpath : " << path << std::endl;
			if (is_file_accessible(path))
			{
				build_response(path);
			}
		}
	}
	else
	{
		std::cout << "Error 404" << std::endl;
	}


	
	
	//if (check_location())

}

void	Process::build_response(std::string path)
{
		_response.set_protocol("HTTP/1.1");
		_response.set_status_code("200");
		_response.set_status_text("OK");

		_response.set_server(_config.getConfigurationKeysWithType(SERVER_NAME).front().server_names.front());
		_response.set_body(get_file_content(path));
		_response.set_content_length(to_str(_response.get_body().length()));
		_response.set_content_type(_response.get_file_format());
		_response.create_response();
}

void	Process::create_index(void)
{
	_response.set_protocol("HTTP/1.1");
	_response.set_status_code("200");
	_response.set_status_text("OK");

	_response.set_server(_config.getConfigurationKeysWithType(SERVER_NAME).front().server_names.front());
	_response.set_content_type("text/html");
	_response.set_body(get_file_content(_config.getConfigurationKeysWithType(INDEX).front().indexes.front()));
	_response.set_content_length(to_str(_response.get_body().length()));
	_response.create_response();
}


bool	Process::check_location(void)
{
	std::vector<ConfigurationKey>	locations = _config.getConfigurationKeysWithType(LOCATION);
	std::cout << "location-vector: " << locations[0].location << std::endl;
	std::vector<ConfigurationKey>::iterator	it;
	for (it = locations.begin(); it != locations.end(); it++)
	{
		std::string	request_path = _request.getPath().first.insert(0, "/");
		std::cout << "config-location: " << (*it).location << std::endl;
		std::cout << "request-location: " << request_path << std::endl;
		if (!(*it).location.compare(request_path))
		{
			return true;
		}
	}
	return false;
}

// ConfigurationKey	Process::get_location(std::string location)
// {
	
// 	std::vector<ConfigurationKey>	locations = _config.getConfigurationKeysWithType(LOCATION);
// 	std::vector<ConfigurationKey>::iterator	it;
// 	ConfigurationKey key(*locations.begin());
// 	for (it = locations.begin(); it != locations.end(); it++)
// 	{
// 		if ((*it).location.compare(location))
// 			key = *it;
// 	}
// 	return key;
// }

std::string	Process::get_location(std::string location, ConfigurationKeyType type)
{
	
	std::vector<ConfigurationKey>	locations = _config.getConfigurationKeysWithType(LOCATION);
	std::vector<ConfigurationKey>::iterator	it;
	std::string	path;
	for (it = locations.begin(); it != locations.end(); it++)
	{
		if (!(*it).location.compare(location))
		{
			if (type == ROOT)
				return (*it).root;
			else if (type == INDEX)
				return (*it).indexes.front();
		}
	}
	return path;
}


// int	Process::get_location(std::string location)
// {
// 	int	i = 0;
// 	std::vector<ConfigurationKey>	locations = _config.getConfigurationKeysWithType(LOCATION);
// 	std::vector<ConfigurationKey>::iterator	it;
// 	for (it = locations.begin(); it != locations.end(); it++, i++)
// 	{
// 		if ((*it).location.compare(location))
// 			return i;
// 	}
// 	return -1;
// }

