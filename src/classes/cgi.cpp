#include "../../inc/cgi.hpp"

CGI::CGI()
{

}

CGI::CGI(/*Request &request*/Request request) : _request(request)
{
	// env[0] = strcat("SERVER_SOFTWARE=", "webserv");						//The name and version of the information server software answering the request (and running the gateway). Format: name/version 
	// env[1] = strcat("SERVER_NAME=", "petroulette");						//The server's hostname, DNS alias, or IP address as it would appear in self-referencing URLs. 
	// env[2] = strcat("GATEWAY_INTERFACE=", "CGI/1.1");					//The revision of the CGI specification to which this server complies. Format: CGI/revision
	// env[3] = strcat("SERVER_PROTOCOL=", "http/1.1");					//The name and revision of the information protcol this request came in with. Format: protocol/revision
	// env[4] = strcat("SERVER_PORT=", request.port);						//The port number to which the request was sent.
	// env[5] = strcat("REQUEST_METHOD=", request.method);					//The method with which the request was made. For HTTP, this is "GET", "HEAD", "POST", etc. 
	// env[6] = strcat("PATH_INFO=", request.path_info);					//The extra path information, as given by the client. In other words, scripts can be accessed by their virtual pathname, followed by extra information at the end of this path. The extra information is sent as PATH_INFO. This information should be decoded by the server if it comes from a URL before it is passed to the CGI script.
	// env[7] = strcat("PATH_TRANSLATED=", request.virtual_path);			//The server provides a translated version of PATH_INFO, which takes the path and does any virtual-to-physical mapping to it. 
	// env[8] = strcat("SCRIPT_NAME=", request.script_path);				//A virtual path to the script being executed, used for self-referencing URLs.
	// env[9] = strcat("QUERY_STRING=", request.query_string);				//The information which follows the ? in the URL which referenced this script. This is the query information. It should not be decoded in any fashion. This variable should always be set when there is query information, regardless of command line decoding. 
	// env[10] = strcat("REMOTE_HOST=", request.host_name);				//The hostname making the request. If the server does not have this information, it should set REMOTE_ADDR and leave this unset.
	// env[11] = strcat("REMOTE_ADDR=", request.host_ip);					//The IP address of the remote host making the request. 
	// env[12] = strcat("AUTH_TYPE=", request.auth);						//If the server supports user authentication, and the script is protects, this is the protocol-specific authentication method used to validate the user.
	// env[13] = strcat("EMOTE_USER=", request.user);						//If the server supports user authentication, and the script is protected, this is the username they have authenticated as. 
	// env[14] = strcat("REMOTE_IDENT=", request.ident);					//If the HTTP server supports RFC 931 identification, then this variable will be set to the remote user name retrieved from the server. Usage of this variable should be limited to logging only. 
	// env[15] = strcat("CONTENT_TYPE=", request.content_type);			//For queries which have attached information, such as HTTP POST and PUT, this is the content type of the data.
	// env[16] = strcat("CONTENT_LENGTH=", request.content_length;)		//The length of the said content as given by the client.


	if (_request.getQuery().second)
	{
		_query_parameters = split_string(_request.getQuery().first, '&');
	}


	_env["SERVER_SOFTWARE"] = "webserv";
	_env["SERVER_NAME"] = "petroulette";
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["SERVER_PROTOCOL"] = "http/1.1";
	_env["SERVER_PORT"] = "8080";
	_env["REQUEST_METHOD"] = "GET";
	_env["PATH_INFO"] = "CGI_bin/script";
	_env["PATH_TRANSLATED"] = "Users/home/Projects/webserv/CGI_bin/script";
	_env["QUERY_STRING"] = "name=Steffen";
	_env["REMOTE_HOST"] = "client";
	_env["REMOTE_ADDR"] = "129.187.214.1";
	_env["AUTH_TYPE"] = "Basic";
	_env["REMOTE_USER"] = "REMOTE_USER";
	_env["REMOTE_IDENT"] = "";
	_env["CONTENT_TYPE"] = "";
	_env["CONTENT_LENGTH"] = "";
}

CGI::~CGI()
{

}

/*executes cgi*/
void	CGI::execute(void)
{
	pid_t	pid;
	
	sleep(5);
	std::cout << "test" << std::endl;
	_tmpout = tmpfile();											//File pointer to a temporaryfile
	sleep(5);
	//_fd = fileno(_tmpout);											//extract the filedescriptor from the file stream
	//_tmpin = tmpfile();
	
	pid = fork();												//forks a new process
	
	if (pid < 0)												//return in case it failes
		return ;
	else if (pid == 0)											//in the child process
	{
		
		//dup2(fileno(_tmpin), STDIN_FILENO);
		dup2(fileno(_tmpout), STDOUT_FILENO);								//stdout now points to the tmpfile
		//char	*argv[5] = {"php-cgi", "echo.php", "firstname=Paul", "lastname=Fritz", NULL};	//creating the arguments for execve
		
		_query_parameters.insert(_query_parameters.begin(), "echo.php");
		_query_parameters.insert(_query_parameters.begin(), "php-cgi");
		
		char	*argv[_query_parameters.size() + 1];
		size_t i = 0;
		std::vector<std::string>::iterator it;
		for (it = _query_parameters.begin(); it != _query_parameters.end(); ++it)
		{
			argv[i] = strdup(to_str(*it).c_str());
			i++;
		}
		argv[i] = NULL;
		//std::cout << "envp: " << map_to_array(_env)[0] << std::endl;
		execve(argv[0], argv, map_to_array(_env));						//executes the executable with its arguments
		//execve("php-cgi", vec_to_array(_query_parameters), map_to_array(_env));
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