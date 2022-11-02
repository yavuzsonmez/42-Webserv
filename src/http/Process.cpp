# include	"../../inc/http/Process.hpp"
# include	"../../inc/debugger/DebuggerPrinter.hpp"

Process::Process(void)
{
	
}

Process::Process(Response &response, Request request, ServerBlock &config) : _response(response), _request(request), _config(config)
{
	_cgi = _config.getCgiPath();
	_cgi_fileending = _config.getCgiFileEnding();
}

Process::~Process(void)
{

}

/**
 * @brief  Process request and handle method type
 * 
 * TODO: Check if given request method is allowed, otherwise sent back a method forbidden page.
 * 
 */
void	Process::process_request(void)
{
	int	method = _request.getMethod().first;
	if (method == GET || method == POST || method == DELETE || method == PUT)
	{
		try {
			get_request();}
		catch (int e) {
			throw (e);}
	}
	else
	{
		throw (501);
	}
}

/**
 * @brief Handles request
 * TODO: Add method enum to handle all request in one place
 * 
 */
void	Process::get_request(void)
{
	std::string	path;
	if (_request.getPath().first == "/")
	{
		if (_request.getScript().first.empty())
		{
			path = _config.getConfigurationKeysWithType(ROOT).front().root + "/" + _config.getConfigurationKeysWithType(INDEX).front().indexes.front();
			try {
				build_response(path, "200", "OK");}
			catch (int e){
				throw (e);}
		}
		else
		{
			path = _config.getConfigurationKeysWithType(ROOT).front().root + "/" + _request.getScript().first;
			try {
				build_response(path, "200", "OK");}
			catch (int e){
				throw (e);}
		}
		return ;

	}
	else if (check_location())
	{
		if (_request.getScript().first.empty())
		{
			
			if (get_location_dl(_request.getPath().first.insert(0, "/")) && get_location(_request.getPath().first.insert(0, "/"), INDEX).empty())
			{
				try {
					build_dl_response();}
				catch (int e){
					throw (e);}
			}
			else
			{
				path = get_location(_request.getPath().first.insert(0, "/"), ROOT) + "/" + get_location(_request.getPath().first.insert(0, "/"), INDEX);
				if (find_vector(_methods, _request.getMethod().first) == -1)
					throw (501);
				try {
					build_response(path, "200", "OK");}
				catch (int e){
					throw (e);}
			}
		}
		else
		{
			path = get_location(_request.getPath().first.insert(0, "/"), ROOT) + "/" + _request.getScript().first;
			if (find_vector(_methods, _request.getMethod().first) == -1)
				throw (501);
			if (is_file_accessible(path))
			{
				try {
					build_response(path, "200", "OK");}
				catch (int e){
					throw (e);}
			}
			else
				throw(404);
		}
	}
	else
	{
		throw(404);
	}
}

void	Process::build_response(std::string path, std::string code, std::string status)
{
		_response.set_protocol("HTTP/1.1");
		if (!_redirection.empty())
		{
			_response.set_status_code("301");
			_response.set_status_text("Moved Permanently");
			_response.set_redirection(_redirection);
		}
		else
		{
			_response.set_status_code(code);
			_response.set_status_text(status);
			_response.set_server(_config.getConfigurationKeysWithType(SERVER_NAME).front().server_names.front());
			if (!path.substr(path.find_last_of(".") + 1).compare(_cgi_fileending))
			{
				CGI	cgi(_request, _config, path, _cgi);
				try {
					cgi.execute();}
				catch (int e) {
					throw(e);}
				_response.set_body(cgi.get_buf());
			}
			else
			{
				try {
					_response.set_body(get_file_content(path));}
				catch (int e){
					throw (500);}
			}
			_response.set_content_length(to_str(_response.get_body().length()));
			_response.set_content_type(_response.get_file_format());
		}
		_response.create_response();
}

void	Process::build_dl_response(void)
{
	std::string	directory;
	char	tmp[1000];
	getcwd(tmp, 1000);
	std::string abs(tmp);
	directory = "\n" + abs + "/" + get_location(_request.getPath().first.insert(0, "/"), ROOT) + "&" + _request.getPath().first;
	_request.setBody(directory);
	
	_response.set_protocol("HTTP/1.1");
	_response.set_status_code("200");
	_response.set_server(_config.getConfigurationKeysWithType(SERVER_NAME).front().server_names.front());
	CGI	cgi(_request, _config, "./directory_listing/directory_listing.php", "php-cgi");
	try {
		cgi.execute();}
	catch (int e) {
		throw(e);}
	_response.set_body(cgi.get_buf());
	_response.set_content_length(to_str(_response.get_body().length()));
	_response.set_content_type(_response.get_file_format());
	_response.create_response();
}

bool	Process::check_location(void)
{
	std::vector<ConfigurationKey>	locations = _config.getConfigurationKeysWithType(LOCATION);
	std::vector<ConfigurationKey>::iterator	it;
	for (it = locations.begin(); it != locations.end(); it++)
	{
		std::string	request_path = _request.getPath().first.insert(0, "/");
		
		if (!(*it).value.compare(request_path))
		{
			return true;
		}
	}
	return false;
}

std::string	Process::get_location(std::string location, ConfigurationKeyType type)
{
	std::vector<ConfigurationKey>	vec = _config.getConfigurationKeysWithType(LOCATION);
	std::vector<ConfigurationKey>::iterator	it;
	std::string	path;
	for (it = vec.begin(); it != vec.end(); it++)
	{
		if (!(*it).value.compare(location))
		{
			if (!(*it).cgi_path.empty())
				_cgi = (*it).cgi_path; 
			if (!(*it).cgi_fileending.empty())
				_cgi_fileending = (*it).cgi_fileending;
			if (!(*it).redirection.empty())
				_redirection = (*it).redirection;
			if (!(*it).allowedMethods.empty())
				_methods = (*it).allowedMethods;
			if (type == ROOT)
				return (*it).root;
			else if (type == INDEX)
			{
				if ((*it).indexes.empty())
				{
					std::string empty;
					return empty;
				}
				return (*it).indexes.front();
			}
		}
	}
	return path;
}

bool	Process::get_location_dl(std::string location)
{
	std::vector<ConfigurationKey>	vec = _config.getConfigurationKeysWithType(LOCATION);
	std::vector<ConfigurationKey>::iterator	it;
	for (it = vec.begin(); it != vec.end(); it++)
	{
		if (!(*it).value.compare(location))
			return (*it).directory_listing;
	}
	return false;
}

void	Process::exception(int e)
{
	switch (e)
	{
		case 404:
			try {
				build_response("default_pages/404_default.html", "404", "Not Found");}
			catch (int e) {
				throw (e);}
			break;
		case 405:
			try {
				build_response("default_pages/405_default.html", "405", "Method Not Allowed");}
			catch (int e) {
				throw (e);}
			break;
		case 500:
			try {
				build_response("default_pages/500_default.html", "500", "Internal Server Error");}
			catch (int e) {
				throw (e);}
			break;
		case 501:
			try {
				build_response("default_pages/501_default.html", "501", "Not Implemented");}
			catch (int e) {
				throw (e);}
			break;
		case 502:
			try {
				build_response("default_pages/502_default.html", "502", "Bad Gateway");}
			catch (int e) {
				throw (e);}
			break;
		case 504:
			try {
				build_response("default_pages/504_default.html", "504", "Gateway Timeout");}
			catch (int e) {
				throw (e);}
			break;
	}
}