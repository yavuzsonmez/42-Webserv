#include "../../inc/http/Cgi.hpp"

CGI::CGI()
{

}

CGI::CGI(Request request, ServerBlock config, std::string path, std::string cgi_path) : _request(request), _config(config), _path(path), _cgi_path(cgi_path)
{
	if (_request.getQuery().second)
	{
		std::string	query = _request.getQuery().first;
		_query_parameters = split_on_delimiter(query, '&');
	}

	_env["SERVER_SOFTWARE"] = "webserv";											//The name and version of the information server software answering the request (and running the gateway). Format: name/version 
	_env["SERVER_NAME"] = _config.getAllServerNames().front();					//The server's hostname, DNS alias, or IP address as it would appear in self-referencing URLs.
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";										//The revision of the CGI specification to which this server complies. Format: CGI/revision
	_env["SERVER_PROTOCOL"] = _request.getProtocol().first;							//The name and revision of the information protcol this request came in with. Format: protocol/revision
	_env["SERVER_PORT"] = to_str(_request.getPort().first);									//The port number to which the request was sent.
	_env["REQUEST_METHOD"] = _request.getMethod().first;							//The method with which the request was made. For HTTP, this is "GET", "HEAD", "POST", etc.
	_env["PATH_INFO"] =  get_abs_path(path);														//The extra path information, as given by the client. In other words, scripts can be accessed by their virtual pathname, followed by extra information at the end of this path. The extra information is sent as PATH_INFO. This information should be decoded by the server if it comes from a URL before it is passed to the CGI script.
	_env["PATH_TRANSLATED"] =  get_abs_path(path);													//The server provides a translated version of PATH_INFO, which takes the path and does any virtual-to-physical mapping to it.
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

	_envp = map_to_array(_env);
}

CGI::~CGI()
{
	//delete (_envp);
}

CGI & CGI::operator=(const CGI &src)
{
	_request = src._request;
	_config = src._config;
	_path = src._path;
	_cgi_path = src._cgi_path;
	_envp = src._envp;
	return *this;
}

void	CGI::set_tmps(void)
{
	_tmp_in = fileno(tmpfile());
	_tmp_out = fileno(tmpfile());
	// pipe(_pipefd_in);
	// pipe(_pipefd_out);
	if (_tmp_in == -1 || _tmp_out == -1)
		throw (500);
	fcntl(_tmp_in, F_SETFL, fcntl(_tmp_in, F_GETFL, 0) | O_NONBLOCK);
	fcntl(_tmp_out, F_SETFL, fcntl(_tmp_out, F_GETFL, 0) | O_NONBLOCK);
	// fcntl(_pipefd_out[0], F_SETFL, fcntl(_pipefd_out[0], F_GETFL, 0) | O_NONBLOCK);
	// fcntl(_pipefd_out[1], F_SETFL, fcntl(_pipefd_out[1], F_GETFL, 0) | O_NONBLOCK);
}

int	CGI::write_in_std_in()
{
	write(_tmp_in, _request.getBody().first.data(), _request.getBody().first.length());
	//close(_pipefd_in[1]);
	return 0;
}

int	CGI::write_in_std_out(void)
{
	int	pid = fork();												//forks a new process

	if (pid < 0)												//return in case it failes
		throw (500);
	else if (pid == 0)											//in the child process
	{
		pid = fork();
		if (pid == 0)
		{
			sleep(2);
			pid_t	ppid = getppid();
			if (pid != 1)
				kill(ppid, SIGKILL);
			exit(EXIT_SUCCESS);
		}
		else
		{
			dup2(_tmp_in, STDIN_FILENO);
			dup2(_tmp_out, STDOUT_FILENO);						//stdout now points to the tmpfile
			_query_parameters.insert(_query_parameters.begin(), _path.c_str());
			_query_parameters.insert(_query_parameters.begin(), _cgi_path.c_str());
			_argvp = vec_to_array(_query_parameters);
			execve(_cgi_path.c_str(), _argvp, _envp);		//executes the executable with its arguments
			kill(pid, SIGKILL);
			exit(EXIT_SUCCESS);												//exit the childprocess
		}
	}
	else														//int the parent process
	{
		close(_tmp_in);
		//close(_pipefd_out[1]);
		int	status;
		waitpid(pid, &status, 0);								//wait until child terminates
		if (WEXITSTATUS(status))
			throw (502);
		else if (WIFSIGNALED(status))
			throw (504);
	}
	return 0;
}

int	CGI::read_in_buff(void)
{
	_buf.resize(100000);									//inrease the underlying char array in _buf by the value of _tmp_size
	read(_tmp_out, (char*)(_buf.data()), 100000);		//read the data from tmpfile into the char array of _buf
	close(_tmp_out);
	return 0;
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