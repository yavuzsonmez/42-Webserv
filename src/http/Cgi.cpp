#include "../../inc/http/Cgi.hpp"
#include "../../inc/debugger/DebuggerPrinter.hpp"

CGI::CGI()
{

}

/**
 * @brief Sets all the environment variables for the CGI script.
 * 
 * @param request The request to handle 
 * @param config The server block configuration of the request
 * @param path the path 
 * @param cgi_path the cgi path
 */
void CGI::set_environment()
{
	_env["SERVER_SOFTWARE"] = "webserv/1.0";											//The name and version of the information server software answering the request (and running the gateway). Format: name/version 
	_env["SERVER_NAME"] = _config.getAllServerNames().front();					//The server's hostname, DNS alias, or IP address as it would appear in self-referencing URLs.
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";										//The revision of the CGI specification to which this server complies. Format: CGI/revision
	_env["SERVER_PROTOCOL"] = _request.getProtocol().first;							//The name and revision of the information protcol this request came in with. Format: protocol/revision
	_env["SERVER_PORT"] = to_str(_request.getPort().first);									//The port number to which the request was sent.
	_env["REQUEST_METHOD"] = _request.getMethod().first;							//The method with which the request was made. For HTTP, this is "GET", "HEAD", "POST", etc.
	_env["PATH_INFO"] =  get_abs_path(_path);														//The extra path information, as given by the client. In other words, scripts can be accessed by their virtual pathname, followed by extra information at the end of this path. The extra information is sent as PATH_INFO. This information should be decoded by the server if it comes from a URL before it is passed to the CGI script.
	_env["PATH_TRANSLATED"] =  get_abs_path(_path);													//The server provides a translated version of PATH_INFO, which takes the path and does any virtual-to-physical mapping to it.
	_env["SCRIPT_NAME"] = "";														//A virtual path to the script being executed, used for self-referencing URLs.
	_env["QUERY_STRING"] = _request.getQuery().first;								//The information which follows the ? in the URL which referenced this script. This is the query information. It should not be decoded in any fashion. This variable should always be set when there is query information, regardless of command line decoding.
	_env["REMOTE_HOST"] = "";														//The hostname making the request. If the server does not have this information, it should set REMOTE_ADDR and leave this unset.
	_env["REMOTE_ADDR"] = "";														//The IP address of the remote host making the request. 
	_env["AUTH_TYPE"] = "";															//If the server supports user authentication, and the script is protects, this is the protocol-specific authentication method used to validate the user.
	_env["REMOTE_USER"] = "";														//If the server supports user authentication, and the script is protected, this is the username they have authenticated as. 
	_env["REMOTE_IDENT"] = "";														//If the HTTP server supports RFC 931 identification, then this variable will be set to the remote user name retrieved from the server. Usage of this variable should be limited to logging only. 
	_env["CONTENT_TYPE"] = _request.findHeader("Content-Type");						//For queries which have attached information, such as HTTP POST and PUT, this is the content type of the data.
	_env["CONTENT_LENGTH"] = _request.findHeader("Content-Length");					//The length of the said content as given by the client.
	_env["REDIRECT_STATUS"] = "500";
}

/**
 * @brief Construct a new CGI::CGI object
 * - Parses the query parameters
 * - Sets the environment variables
 * 
 * @param request The request to handle 
 * @param config The server block configuration of the request
 * @param path the path 
 * @param cgi_path the cgi path
 */
CGI::CGI(Request request, ServerBlock config, std::string path, std::string cgi_path) : _request(request), _config(config), _path(path), _cgi_path(cgi_path)
{
	if (_request.getQuery().second)
	{
		std::string	query = _request.getQuery().first; //Get query string
		_query_parameters = split_on_delimiter(query, '&');  //Split query string on '&' to get the parameters of the GET request
	}

	set_environment();
}

CGI::~CGI()
{

}

CGI & CGI::operator=(const CGI &src)
{
	_request = src._request;
	_config = src._config;
	_path = src._path;
	_cgi_path = src._cgi_path;
	return *this;
}

/**
 * @brief creates the tmpfiles and makes their fds nonblocking
 * 
 */
void	CGI::set_tmps(void)
{
	_tmpout = tmpfile();
	_tmpin = tmpfile();
	_fd_in = fileno(_tmpin);
	_fd_out = fileno(_tmpout);
	fcntl(_fd_in, F_SETFL, fcntl(_fd_in, F_GETFL, 0) | O_NONBLOCK);
	fcntl(_fd_out, F_SETFL, fcntl(_fd_out, F_GETFL, 0) | O_NONBLOCK);
}

/**
 * @brief writes the body from the request in _tmp_in which later will be dupped to the STDIN
 * Takes the _fd_in and writes to it the body of the request
 */
void	CGI::write_in_std_in()
{
	if (_fd_in < 0)
		return;
	write(_fd_in, _request.getBody().first.data(), _request.getBody().first.length());
	if (_tmpin != NULL)
		rewind(_tmpin);
	return ;
}

/**
 * @brief Here we wait for the child.
 * If it takes too long, it will timeout
 */
void CGI::wait_for_child(pid_t worker_pid)
{
	USE_DEBUGGER;
	pid_t timeout_pid = fork();
	if (timeout_pid < 0)
	{
		debugger.error("[TIMEOUT] fork failed in timeout pid");
		kill(worker_pid, SIGKILL);
		throw(500);
	}
	else if (timeout_pid == 0) // timeout child
	{
		sleep(5);
		std::exit(EXIT_SUCCESS);
	}
	else // parent
	{
		int stat_loc = 0;
		pid_t pid = 0;

		while ((pid = waitpid(worker_pid, &stat_loc, WNOHANG)) == 0 &&
				(pid = waitpid(timeout_pid, &stat_loc, WNOHANG)) == 0)
			usleep(50);

		if (pid == -1)
		{
			debugger.error("[TIMEOUT] waitpid failed"); // should never happen, I think but actually I don't know
			kill(worker_pid, SIGKILL);
			kill(timeout_pid, SIGKILL);
			throw(500);
		}
		else if (pid == worker_pid) // this is being called if the worker finishes on time
		{
			kill(timeout_pid, SIGKILL); // let's kill the timeout child, it's not needed anymore
			if (!WIFSIGNALED(stat_loc) && WEXITSTATUS(stat_loc) == 0) // if the worker exited normally
				return ; // we're done
			return ; 
		}
		else
		{
			debugger.error("[TIMEOUT] cgi gateway error in wait_for_child"); // this is being called if the worker timed out
			kill(worker_pid, SIGKILL); // let's kill the worker
			throw(504); // that is a gateway timeout
		}
	}
}

/**
 * @brief executes the cgi an writes the return of it into _tmp_out
 * TODO: Fork leak!
 */
void	CGI::execute_cgi(void)
{
	USE_DEBUGGER;
	pid_t pid = fork();
	if (pid < 0)
	{
		debugger.error("Could not fork CGI.");
		throw(500);
	}
	else if (pid == 0) // child
	{
		if (dup2(fileno(_tmpin), STDIN_FILENO) < 0 ||
			dup2(fileno(_tmpout), STDOUT_FILENO) < 0)
		{
			debugger.error("Failed to dup2 the CGI.");
			std::exit(errno); // exit the child
		}
		char** envs = map_to_array(_env);
		_query_parameters.insert(_query_parameters.begin(), _path.c_str());
		_query_parameters.insert(_query_parameters.begin(), _cgi_path.c_str());
		_argvp = vec_to_array(_query_parameters);
		execve(_cgi_path.c_str(), _argvp, envs);
		debugger.error("Could not execute CGI. Error happened in execute_cgi");
		std::exit(errno); // exit the child
	}
	else // parent
		return wait_for_child(pid);

	//try {
	//	int	pid = fork();												//forks a new process

	//	if (pid < 0)												//return in case it failes
	//		throw (500);
	//	else if (pid == 0)											//in the child process
	//	{
	//		pid = fork();
	//		if (pid < 0)												//return in case it failes
	//			throw (500);
	//		if (pid == 0) // timeout handling. for that we are using a fork
	//		{
	//			sleep(2);
	//			pid_t	ppid = getppid();
	//			debugger.debug("CGI timeout: killing pid " + std::to_string(ppid));
	//			if (pid != 1)
	//				kill(ppid, SIGKILL);
	//			exit(EXIT_SUCCESS);
	//		}
	//		else  // execute the php in the child
	//		{
	//			int check1 = dup2(_fd_in, STDIN_FILENO);
	//			int check2 = dup2(_fd_out, STDOUT_FILENO);						//stdout now points to the tmpfile
	//			if (check1 == -1 || check2 == -1)
	//				throw (500);
	//			_query_parameters.insert(_query_parameters.begin(), _path.c_str());
	//			_query_parameters.insert(_query_parameters.begin(), _cgi_path.c_str());
	//			_argvp = vec_to_array(_query_parameters);
	//			execve(_cgi_path.c_str(), _argvp, _envp);		//executes the executable with its arguments
	//			kill(pid, SIGKILL);
	//			exit(EXIT_SUCCESS);
	//		}
	//	}
	//	else														//int the parent process
	//	{
	//		int	status;
	//		waitpid(pid, &status, 0);								// wait until child terminates, status of the pid gets written in status variable
	//		close(_fd_in);
	//		if (WEXITSTATUS(status))
	//			throw (502);
	//		else if (WIFSIGNALED(status))
	//			throw (504);
	//	}
	//	return ;
	//} catch (int error) {
	//	debugger.error("CGI error: " + std::to_string(error));
		
	//	return ;
	//}
}

/**
 * @brief writes the return of the cgi into the body of the responseo
 */
void	CGI::read_in_buff(void)
{
	USE_DEBUGGER;
	if (_fd_in < 0)
		return;
	try {
		if (fseek(_tmpout, 0, SEEK_END) < 0)							//set the courser in the filestream to the end
			throw (500);
		if ((_tmp_size = ftell(_tmpout)) == -1)								//assign the position of the courser to _tmp_size
			throw (500);
	} catch (int error) {
		debugger.error("CGI error: " + std::to_string(error));
		close(_fd_out);
		throw (501);
		return ;
	}
	rewind(_tmpout);											//move the courser back to the beginning
	_buf.resize(_tmp_size);									//inrease the underlying char array in _buf by the value of _tmp_size
	read(_fd_out, (char*)(_buf.data()), _tmp_size);
	close(_fd_out);
	return ;
}

std::string	CGI::get_buf(void)
{
	std::string	ret(_buf);
	return ret;
}

std::string	CGI::get_query(std::string referer)
{
	return	referer.substr(referer.find('?') + 1);
}