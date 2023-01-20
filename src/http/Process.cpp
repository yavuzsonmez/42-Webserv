# include	"../../inc/http/Process.hpp"
# include	"../../inc/debugger/DebuggerPrinter.hpp"

Process::Process()
{

}

Process::Process(Request request, ServerBlock config) : _request(request), _config(config)
{
	_with_cgi = false;
	_cgi_path = _config.getCgiPath();
	_cgi_fileending = _config.getCgiFileEnding();
	ConfigurationKey locationKey;
	if (set_location_key_if_exists(locationKey)) {
		_cgi_path = locationKey.cgi_path;
		_cgi_fileending = locationKey.cgi_path;
	}
	_server_name = request.getHost();
}

Process::~Process(void)
{
	USE_DEBUGGER;
}

Process & Process::operator = (const Process &src)
{
	_request = src._request;
	_config = src._config;
	_cgi_path = src._cgi_path;
	_cgi_fileending = src._cgi_fileending;
	_with_cgi = src._with_cgi;
	_server_name = src._server_name;
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
			handle_request();
		}
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
 * @brief Gets the path for a script or index file in a directory, but also checks if the request is a directory and handles it accordingly.
 * @return std::string 
 */
std::string Process::getPathForNestedLocation()
{
	USE_DEBUGGER;
	if (!_request.hasNestedRequestPath) { // if the path is not nested
		if (_request.getScript().first.empty()) { // there is no additional script like /echo.php
			return _config.getConfigurationKeysWithType(ROOT).front().root  + _request.getPath().first + "/" + _config.getConfigurationKeysWithType(INDEX).front().indexes.front();
		} else { // there is a script file available like /echo.php
			return _config.getConfigurationKeysWithType(ROOT).front().root + _request.getPath().first + "/" + _request.getScript().first;
		}
	} else { // we do not provide the index files for a nested path
		// here we build the nested path for the location. this needs to be put in a seperate function later for sure...
		// we use get_location to get the correct root path
		std::string primaryLocation = get_first_location_in_path(_request.getPath().first); // where the nested folder is
			// we need to add a slash to the location to get the correct root path and a slash after to match the location of the configuration file
			// TODO Generate the full path for the nested path
		std::string root = get_location(primaryLocation.insert(0, "/") + "/", ROOT);
		// now append the rest of the path to the newly gained root path
		std::string path = root +  "/" + _request.getPath().first.substr(primaryLocation.length());
		if (_request.getScript().first.empty()) { // there is no additional script like /echo.php. Return the index file for the location of the nested path
			return path + "/" + get_location(primaryLocation + "/", INDEX);
		} else { // there is a script file available like /echo.php
			return path + "/" + _request.getScript().first;
		}
	}
}

/**
 * @brief Returns the path with the index file defined in the configuration.
 * Will check every index file if available and accessible and chooses the first one that is accessible
 * @returns std::string path
 */
std::string Process::build_path_with_index__or_script_file()
{
	USE_DEBUGGER;
	if (_request.getPath().first == "/") { // the path is on the top level and not in any subdirectory
		if (_request.getScript().first.empty()) { // there is no additional script like /echo.php
			return _config.getConfigurationKeysWithType(ROOT).front().root + "/" + _config.getConfigurationKeysWithType(INDEX).front().indexes.front();
		} else { // there is a script file available like /echo.php
			return _config.getConfigurationKeysWithType(ROOT).front().root + "/" + _request.getScript().first;
		}
	} else {
		// if it is not a nested path, we can just add the index file to the path or the script file
		return getPathForNestedLocation();
	}
}

/**
 * @brief Check if a request is too large for the server to handle
 * @param request 
 * @return false if request is too large, true if request is not too large
 */
bool Process::check_if_request_is_too_large()
{
	if (_config.getConfigurationKeysWithType(POST_MAX_SIZE).size() > 0) {
		if ((long) _request.getBody().first.size() > (_config.getConfigurationKeysWithType(POST_MAX_SIZE).front().post_max_size * 1000000))
		{
			std::cout << "Request too big" << std::endl;
			exception(413);
			return false;
		}
		return true;
	}
	return true;
}

/**
 * @brief Handles the request.
 * Checks if the request is too large, if the request is a cgi request and if the request is a normal request.
 * Will throw an exception if something goes wrong.
 * 
 */
void	Process::handle_request(void)
{
	USE_DEBUGGER;
	std::string	path;

	// check if request is too large
	if (check_if_request_is_too_large() == false)
		return exception(413);
	
	// first we check if the request is too big
	if (_request.getPath().first == "/") // if the script is the root path
	{
		path = build_path_with_index__or_script_file(); // we return the file looked for or the index file
		try {
			build_response(path, "200", "OK");}
		catch (int e){
			debugger.error("UNABLE TO BUILD RESPONSE!");
			throw(404);
			return ;
		}
		return ;
	}
	else if (check_location()) // here we check if the location exists (target directory)
	{
		if (_request.getScript().first.empty()) // if no script is given
		{
			if (get_location_dl(_request.getPath().first.insert(0, "/")) && get_location(_request.getPath().first.insert(0, "/"), INDEX).empty())
			{
				if (find_vector(_methods, _request.getMethod().first) == -1)
					throw (405);
				try {
					build_dl_response();
				}
				catch (int e){
					debugger.error("Could not find the file listing script!");
					throw(404);
					return ;
				}
			}
			else // if not, we try to return the index file
			{
				path = get_location(_request.getPath().first.insert(0, "/"), ROOT) + "/" + get_location(_request.getPath().first.insert(0, "/"), INDEX);
				if (find_vector(_methods, _request.getMethod().first) == -1)
					throw (405);
				try {
					build_response(path, "200", "OK");}
				catch (int e){
					debugger.error("Could not find the index script!");
					throw(404);
					return ;
				}
			}
		}
		else
		{
			path = get_location(_request.getPath().first.insert(0, "/"), ROOT) + "/" + _request.getScript().first;
			if (find_vector(_methods, _request.getMethod().first) == -1)
				throw (405);
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
		// nested directory
		path = build_path_with_index__or_script_file();
		removeDoubleSlashesInUrl(path);
		//if (find_vector(_methods, _request.getMethod().first) == -1)
		//	throw (405);
		try {
			build_response(path, "200", "OK");}
		catch (int e){
			debugger.error("UNABLE TO BUILD RESPONSE!");
			throw(404);
			return ;
		}
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
 * @brief Returns true if an CGI was detected
 * 
 * @param path 
 * @param code 
 * @param status 
 * @return true 
 * @return false 
 */
bool Process::detectCgi(std::string path, std::string code, std::string status)
{
	USE_DEBUGGER;
	(void) status;
	(void) code; 
	// checks if the requested file is an file which has an cgi extension. check if before the ? there is a .cgi ending
	std::string::size_type pos = path.find("?");
	std::string first_part = path.substr(0, pos); // then we get the file name

	if (!is_file_accessible(get_abs_path(path)))
	{
		debugger.error("File not found!");
		return false;
	}
	// check if the file ending is the cgi ending but there are arguments too
	if (!first_part.substr(first_part.find_last_of(".") + 1).compare(_cgi_fileending)) {
		debugger.verbose("DETECTED CGI!");
		return true;
	}

	// we also need to check if there is no script given and if the index.php contains what we want.
	if (_request.getScript().first.empty()) {
		debugger.verbose("DETECTED EMPTY SCRIPT!");
	}

	// check if the file ending is the cgi ending but there no arguments
	if (!path.substr(path.find_last_of(".") + 1).compare(_cgi_fileending)) {
		debugger.verbose("DETECTED CGI!");
		return true;
	}
	return false;
}

/**
 * @brief sets the header and the body of the response.
 * - In case of a cgi it creates a cgi object and returns.
 * - In case of a redirection it sets the redirection response.
 */
void	Process::build_response(std::string path, std::string code, std::string status)
{
	USE_DEBUGGER;
	_response.set_protocol("HTTP/1.1");
	if (!_redirection.empty()) // if the redirection is not empty
	{
		set_redirection_response();
	}
	else
	{
		// TEST
		_response.set_status_code(code);
		_response.set_status_text(status);
		_response.set_server(_server_name);
		// This is where cgi is recognized
		if (detectCgi(path, code, status)) // checks if the file ending has the cgi fileending, if yes, the request is targeted to the cgi
		{
			_with_cgi = true;
			_CGI = CGI(_request, _server_name, path, _cgi_path); // activates the cgi
			_CGI.set_tmps(); // sets the tmps for the cgi, so we can output and input to and from the cgi
			return ;
		}
		else // the request is not cgi or redirection, so we just return the content of the file at the location of path.
		{
			// the request is a static file
			try {
				_response.set_body(get_file_content_for_request(path)); // still ok
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
 * @deprecated won't work because fd will be invalid. do not try to send messages to client!
 */
void	Process::server_overloaded(void)
{
	return ;
}

/**
 * @brief builds the response in case of a cgi
 */
void	Process::build_cgi_response(void)
{
	std::string tmp;
	tmp = _CGI.get_buf();
	_response.set_body(tmp);
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
	std::string cgi_path = _config.getCgiPath();
	char	tmp[1000];
	getcwd(tmp, 1000);
	std::string abs(tmp);
	directory = abs + "/" + get_location(_request.getPath().first.insert(0, "/"), ROOT) + "/";
	try {
		_request.setBody(directory);
	} catch (int e) {
		throw (404);
	}
	_response.set_protocol("HTTP/1.1");
	_response.set_status_code("200");
	_response.set_server(_config.getConfigurationKeysWithType(SERVER_NAME).front().server_names.front());
	_with_cgi = true;
	_CGI = CGI(_request, _server_name, "./resources/directory_listing/directory_listing.php", "php-cgi");
	_CGI.location_dl = directory;
	_CGI.set_tmps();
}


/**
 * @brief Sets it to the configuration key, if the location key exists
 * @param &location
 */
bool	Process::set_location_key_if_exists(ConfigurationKey &location)
{
	std::vector<ConfigurationKey>	locations = _config.getConfigurationKeysWithType(LOCATION);
	std::vector<ConfigurationKey>::iterator	it;
	for (it = locations.begin(); it != locations.end(); it++)
	{
		std::string	request_path = _request.getPath().first.insert(0, "/");
		
		if (!(*it).value.compare(request_path))
		{
			location = *it;
			return true;
		}
	}
	return false;
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
				_cgi_path = (*it).cgi_path; 
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
	std::cout << "HANDLING ERROR CODE WITH" << e << std::endl;
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
		case 413:
			try {
				build_response(_config.getErrorPagePathForCode(413), "413", "Request too big");
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