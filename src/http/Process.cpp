# include	"../../inc/http/Process.hpp"
# include	"../../inc/debugger/DebuggerPrinter.hpp"

Process::Process()
{

}

Process::Process(Request request, ServerBlock config) : _request(request), _config(config)
{
	_with_cgi = false;
	_cgi = _config.getCgiPath();
	_cgi_fileending = _config.getCgiFileEnding();
}

Process::~Process(void)
{

}

Process & Process::operator = (const Process &src)
{
	_request = src._request;
	_config = src._config;
	_cgi = src._cgi;
	_cgi_fileending = src._cgi_fileending;
	return *this;
}

/**
 * @brief  Process request and handle method type
 * 
 * TODO: Check if given request method is allowed, otherwise sent back a method forbidden page.
 * Currently accepted methods are GET, POST, DELETE and PUT.
 */
void	Process::process_request(void)
{
	int	method = _request.getMethod().first;
	if (method == GET || method == POST || method == DELETE || method == PUT)
	{
		try {
			handle_request();}
		catch (int e) {
			exception(e);
			return ;
		}
	}
	else
	{
		exception(501);
		return ;
	}
}

/**
 * @brief Handles request
 * TODO: Add method enum to handle all request in one place
 * 
 * TODO: Please improve readability, this is very confusing. It is not clear what does what.
 * Use at least 6 seperate functions for everything done in this function
 * 
 */
void	Process::handle_request(void)
{
	USE_DEBUGGER;
	std::string	path;
	if (_request.getPath().first == "/") // if the script is the root path
	{
		if (_request.getScript().first.empty()) // case: no script and we return the given index if available
		{
			path = _config.getConfigurationKeysWithType(ROOT).front().root + "/" + _config.getConfigurationKeysWithType(INDEX).front().indexes.front();
			try {
				build_response(path, "200", "OK");}
			catch (int e){
				debugger.error("UNABLE TO BUILD RESPONSE!");
				throw(404);
				return ;
			}
		}
		else // this is being called when a script is given and we do find it / do not find it 
		{
			path = _config.getConfigurationKeysWithType(ROOT).front().root + "/" + _request.getScript().first;
			try {
				build_response(path, "200", "OK");}
			catch (int e){
				throw(500);
				return ;
			}
		}
		return ;
	}
	else if (check_location()) // here we check if the location exists (target directory)
	{
		if (_request.getScript().first.empty()) // if no script is given
		{
			// here we use the location directory list to list all files in the directory
			if (get_location_dl(_request.getPath().first.insert(0, "/")) && get_location(_request.getPath().first.insert(0, "/"), INDEX).empty())
			{
				try {
					build_dl_response();}
				catch (int e){
					throw(404);
					return ;
				}
			}
			else
			{
				path = get_location(_request.getPath().first.insert(0, "/"), ROOT) + "/" + get_location(_request.getPath().first.insert(0, "/"), INDEX);
				if (find_vector(_methods, _request.getMethod().first) == -1) // the method is not allowed
					throw(404);
				try {
					build_response(path, "200", "OK");}
				catch (int e){
					throw(404);
					return ;
				}
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
					throw(401);
					return ;
				}
			}
			else
			{
				throw(401);
				return ;
			}
		}
	}
	else
	{
		throw(404);
	}
}

/**
 * @brief Sets the response headers for a redirection
 */
void Process::set_redirection_response()
{
	_response.set_status_code("301");
	_response.set_status_text("Moved Permanently");
	_response.set_redirection(_redirection);
}
/**
 * @brief sets the header and the body of the response.
 * - In case of a cgi it creates a cgi object and returns.
 * - In case of a redirection it sets the redirection response.
 */
void	Process::build_response(std::string path, std::string code, std::string status)
{
		_response.set_protocol("HTTP/1.1");
		if (!_redirection.empty()) // if the redirection is not empty
		{
			set_redirection_response();
		}
		else
		{
			_response.set_status_code(code);
			_response.set_status_text(status);
			_response.set_server(_config.getConfigurationKeysWithType(SERVER_NAME).front().server_names.front());
			if (!path.substr(path.find_last_of(".") + 1).compare(_cgi_fileending)) // checks if the file ending has the cgi fileending, if yes, the request is targeted to the cgi
			{
				_with_cgi = true;
				_CGI = CGI(_request, _config, path, _cgi); // activates the cgi
				_CGI.set_tmps(); // sets the tmps for the cgi, so we can output and input to and from the cgi
				return ;
			}
			else // the request is not cgi or redirection, so we just return the content of the file at the location of path.
			{
				try {
					_response.set_body(get_file_content_for_request(path));
				} catch (int e) {
					throw (404);
				}
			}
			_response.set_content_length(to_str(_response.get_body().length()));
			_response.set_content_type(_response.get_file_format());
		}
		_response.create_response();
}

/**
 * @brief Sends a server overloaded message to the client
 */
void	Process::server_overloaded(void)
{
	_response.set_body("Server overloaded. Go away!");
	_response.set_content_length(to_str(_response.get_body().length()));
	_response.set_content_type(_response.get_file_format());
	_response.create_response();
}

/**
 * @brief builds the response in case of a cgi
 */
void	Process::build_cgi_response(void)
{
	_response.set_body(_CGI.get_buf());
	_response.set_content_length(to_str(_response.get_body().length()));
	_response.set_content_type(_response.get_file_format());
	_response.create_response();
}

/**
 * @brief creates the response for a directory listing using the file at the directory_listing directory.
 * It passes on the path of the directory where we list information on to the file.
 */
void	Process::build_dl_response(void)
{
	std::string	directory;
	char	tmp[1000];
	getcwd(tmp, 1000);
	std::string abs(tmp);
	directory = abs + "/" + get_location(_request.getPath().first.insert(0, "/"), ROOT) + "&" + _request.getPath().first;
	_request.setBody(directory);
	_response.set_protocol("HTTP/1.1");
	_response.set_status_code("200");
	_response.set_server(_config.getConfigurationKeysWithType(SERVER_NAME).front().server_names.front());
	_with_cgi = true;
	_CGI = CGI(_request, _config, "./directory_listing/directory_listing.php", "php-cgi");
	_CGI.set_tmps();
}

/**
 * @brief checks if the given location exists
 */
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

/**
 * @brief gets all the information of a given location
 */
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

/**
 * @brief If an exception is thrown, we have the option to return a error page that fits.
 * @param e 
 */
void	Process::exception(int e)
{
	switch (e)
	{
		case 404:
			try {
				build_response(_config.getErrorPagePathForCode(404), "404", "Not Found");
			}
			catch (int e) {
				throw (e);
			}
			break;
		case 405:
			try {
				build_response(_config.getErrorPagePathForCode(405), "405", "Method not allowed");
			}
			catch (int e) {
				throw (e);
			}
			break;
		case 500:
			try {
				build_response(_config.getErrorPagePathForCode(500), "500", "Internal server error");
			}
			catch (int e) {
				throw (e);
			}
			break;
		case 501:
			try {
				build_response(_config.getErrorPagePathForCode(501), "501", "Not implemented");
			}
			catch (int e) {
				throw (e);
			}
			break;
		case 502:
			try {
				build_response(_config.getErrorPagePathForCode(502), "502", "Bad gateway");
			}
			catch (int e) {
				throw (e);
			}
			break;
		case 504:
			try {
				build_response(_config.getErrorPagePathForCode(504), "504", "Gateway timeout");
			}
			catch (int e) {
				throw (e);
			}
			break;
		default:
			try {
				build_response(_config.getErrorPagePathForCode(503), "503", "Server not available");
			}
			catch (int e) {
				throw (e);
			}
			break;
	}
}