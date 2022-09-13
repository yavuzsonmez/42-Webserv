#include "../../inc/http/Cgi.hpp"

CGI::CGI()
{

}

CGI::CGI(/*Request &request*/Request request, ServerBlock &config) : _request(request), _config(config)
{
	if (_request.getQuery().second)
	{
		std::string	query = _request.getQuery().first;
		_query_parameters = split_on_delimiter(query, '&');
	}

	//_env["SERVER_SOFTWARE"] = "webserv";											//The name and version of the information server software answering the request (and running the gateway). Format: name/version 
	//_env["SERVER_NAME"] = _config.getAllServerNames().front();					//The server's hostname, DNS alias, or IP address as it would appear in self-referencing URLs.
	//_env["GATEWAY_INTERFACE"] = "CGI/1.1";										//The revision of the CGI specification to which this server complies. Format: CGI/revision
	_env["SERVER_PROTOCOL"] = _request.getProtocol().first;							//The name and revision of the information protcol this request came in with. Format: protocol/revision
	_env["SERVER_PORT"] = _request.getPort().first;									//The port number to which the request was sent.
	//_env["REQUEST_METHOD"] = _request.getMethod().first;							//The method with which the request was made. For HTTP, this is "GET", "HEAD", "POST", etc.
	_env["PATH_INFO"] = _request.getPath().first;									//The extra path information, as given by the client. In other words, scripts can be accessed by their virtual pathname, followed by extra information at the end of this path. The extra information is sent as PATH_INFO. This information should be decoded by the server if it comes from a URL before it is passed to the CGI script.
	_env["PATH_TRANSLATED"] = _request.getPath().first;								//The server provides a translated version of PATH_INFO, which takes the path and does any virtual-to-physical mapping to it.
	_env["SCRIPT_NAME"] = "";														//A virtual path to the script being executed, used for self-referencing URLs.
	_env["QUERY_STRING"] = _request.getQuery().first;								//The information which follows the ? in the URL which referenced this script. This is the query information. It should not be decoded in any fashion. This variable should always be set when there is query information, regardless of command line decoding.
	_env["REMOTE_HOST"] = "";														//The hostname making the request. If the server does not have this information, it should set REMOTE_ADDR and leave this unset.
	_env["REMOTE_ADDR"] = "";														//The IP address of the remote host making the request. 
	_env["AUTH_TYPE"] = "";															//If the server supports user authentication, and the script is protects, this is the protocol-specific authentication method used to validate the user.
	_env["REMOTE_USER"] = "";														//If the server supports user authentication, and the script is protected, this is the username they have authenticated as. 
	_env["REMOTE_IDENT"] = "";														//If the HTTP server supports RFC 931 identification, then this variable will be set to the remote user name retrieved from the server. Usage of this variable should be limited to logging only. 
	_env["CONTENT_TYPE"] = "";	//need the content-type from request header			//For queries which have attached information, such as HTTP POST and PUT, this is the content type of the data.
	_env["CONTENT_LENGTH"] = _request.getBody().first.length();						//The length of the said content as given by the client.
}

CGI::~CGI()
{

}

/*executes cgi*/
void	CGI::execute(void)
{
	std::string cgi_path;
	std::string cgi_index;
	
	for (int j = 0; j < (int) _config.configurationKeys.size(); j++)
	{
		if (_config.configurationKeys[j].configurationType == LOCATION)
		{
			cgi_path = _config.configurationKeys[j].cgi_path;
			cgi_index = _config.configurationKeys[j].indexes.front();
		}
	}

	pid_t	pid;
	
	_tmpout = tmpfile();											//File pointer to a temporaryfile
	_tmpin = tmpfile();
	fwrite(_request.getBody().first.data(), 1, _request.getBody().first.length(), _tmpin);
	
	pid = fork();												//forks a new process
	
	if (pid < 0)												//return in case it failes
		return ;
	else if (pid == 0)											//in the child process
	{
		dup2(fileno(_tmpin), STDIN_FILENO);
		dup2(fileno(_tmpout), STDOUT_FILENO);								//stdout now points to the tmpfile
		
		_query_parameters.insert(_query_parameters.begin(), cgi_index.c_str());
		_query_parameters.insert(_query_parameters.begin(), cgi_path.c_str());

		execve(cgi_path.c_str(), vec_to_array(_query_parameters), map_to_array(_env));						//executes the executable with its arguments
		//std::cout << "execve: " << i << std::endl;				//check if execve failes
		exit(1);												//exit the childprocess
	}
	else														//int the parent process
	{
		int	status;
		waitpid(pid, &status, 0);								//wait until child terminates
		fseek(_tmpout, 0, SEEK_END);							//set the courser in the filestream to the end
		_tmp_size = ftell(_tmpout);								//assign the position of the courser to _tmp_size
		rewind(_tmpout);											//move the courser back to the beginning
		_buf.resize(_tmp_size);									//inrease the underlying char array in _buf by the value of _tmp_size
		fread((char*)(_buf.data()), 1, _tmp_size, _tmpout);		//read the data from tmpfile into the char array of _buf
		return;
	}

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