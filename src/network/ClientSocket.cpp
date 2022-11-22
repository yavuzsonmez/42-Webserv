#include "../../inc/network/ClientSocket.hpp"
#include "../../inc/debugger/DebuggerPrinter.hpp"


/**
 * @brief Store all the data related to the client and link it to the forwarded fd
 * Initialize the data we need to track the process for one client to read the request and write the response
 * @param clientSocket, Source information (source port and ip of the client)
 * @param config, Config of the server
 * @param forward, Fd linked to the client (where we will read the request and respond)
 */
ClientSocket::ClientSocket(struct sockaddr_in clientSocket, ServerBlock &config, int forward) : _config(config)
{
	_socket.sin_family = clientSocket.sin_family;
	_socket.sin_port = clientSocket.sin_port;
	_socket.sin_addr.s_addr = clientSocket.sin_addr.s_addr;
	bzero(&(_socket.sin_zero), 8);

	_func_ptr = &ClientSocket::read_in_buffer;
	_state = HEADER;
	_fd = forward;
	_client_fd = forward;
	_position = 0;
	_count = 30000;
	_event = POLLIN;
	_remove = false;
	_timeout = std::time(NULL);
}

ClientSocket::~ClientSocket()
{

}

/**
 * @brief Check if the client if over the defined timeout
 */
bool ClientSocket::Timeout()
{
	if (std::time(NULL) - _timeout > 5000)
		return true;
	return false;
}

/**
 * @brief Track the next function to execute
 */
void	ClientSocket::call_func_ptr(void)
{
	(this->*_func_ptr)();
}

/**
 * @brief reads the requst from the clientSocket until \r\n\r\n.
 * If a Body is appended change the _state = BODY and reads until it reaches content_lenght as it is defined in the header.
 */
void	ClientSocket::read_in_buffer(void)
{
	buffer.resize(buffer.size() + _count);
	_bytes = read(_fd, (char*)buffer.c_str() + _position, _count);
	_position += _bytes;
	if (_state == HEADER)
	{
		size_t pos = buffer.find("\r\n\r\n");
		if (pos != std::string::npos)
		{
			std::string httpRequestHead = buffer.substr(0, pos + 3);
			_clientRequest.parser(httpRequestHead);
			buffer.erase(0, pos + 3);
			_position -= pos + 3;
			_state = BODY;
			_content_length = atoi(_clientRequest.findHeader("Content-Length").c_str());
			if (!_content_length)
			{
				set_up();
				return ;
			}
		}
	}
	else if (_state == BODY)
	{
		if (_position >= _content_length)
		{
			_clientRequest.setBody(buffer);
			set_up();
			return ;
		}
	}
	return ;
}

/**
 * @brief writes the response to the clientSocket
 * We need to check if the bytes are actually zero before we write to the clientSocket.
 * _position keeps track of what the client already received.
 */
void	ClientSocket::send_response(void)
{
	USE_DEBUGGER;
	if (_remove) {
		debugger.verbose("Client wants to be removed!");
		return ;
	};
	if (!is_valid_fd(_fd))
	{
		debugger.verbose("Error 3 while sending response to client");
		_remove = true;
		close(_fd);
		return ;
	}
	_bytes = send(_fd, _process._response.get_response().data() + _position, _process._response.get_response().length() - _position, 0);
	if (_bytes == -1)
	{
		_remove = true;
		debugger.verbose("Error while sending response to client");
		return ;
	}
	_position += _bytes;
	if (_position >= _process._response.get_response().length())
	{
		debugger.debug("Completed sending (chunked) response to client");
		_remove = true;
		return ;
	}
	return ;
}

/**
 * @brief sets up the process object.
 * in case of a cgi, it sets the _func_ptr to the next function that handels the cgi and updates the filedescriptor
 */
void	ClientSocket::set_up(void)
{
	_process = Process(_clientRequest, _config);
	try
	{
		_process.process_request();
	}
	catch (int e)
	{
		_process.exception(e);
		return ;
	}
	if (_process._with_cgi) // in the next step we will write in the cgi input
	{
		_bytes = 0;
		_position = 0;
		_func_ptr = &ClientSocket::one;
		_fd = _process._CGI._fd_in;
		_event = POLLOUT;
	}
	else  // in the next step we will send the response to the client as soon as it is ready
	{
		_bytes = 0;
		_position = 0;
		_func_ptr = &ClientSocket::send_response;
		_event = POLLOUT;
	}
	return ;
}

/**
 * @brief writes in the std in of the cgi.
 */
void	ClientSocket::one(void)
{
	USE_DEBUGGER;
	try {
		_process._CGI.write_in_std_in();
	} catch (int e) {
		_event = POLLERR;
		debugger.verbose("Thrown exception in ::one");
		_process.exception(e);
		return ;
	}
	_fd = _process._CGI._fd_out;
	_event = POLLOUT;
	_func_ptr = &ClientSocket::two;
	return ;
}

/**
 * If writing in the stdin of the cgi is completed, we run it
*/
void	ClientSocket::two(void)
{
	USE_DEBUGGER;
	try {
		std::cout << "running cgi in two" << std::endl;
		_process._CGI.set_environment();
		_process._CGI.execute_cgi();
	} catch (int error) {
		debugger.verbose("Thrown cgi exception in ::two");
		std::cerr << "Error in ::two " << error << std::endl;
		_event = POLLERR;
		_process.server_overloaded();
		return ;
	}
	_fd = _process._CGI._fd_out;
	_event = POLLIN;
	_func_ptr = &ClientSocket::three;
	return ;
}

/**
 * After running the cgi, we send the response to the client.
*/
void	ClientSocket::three(void)
{
	USE_DEBUGGER;
	try {
		_process._CGI.read_in_buff();
	} catch (int error) {
		debugger.verbose("Thrown cgi exception in ::three");
		_event = POLLERR;
		_process.server_overloaded();
		return ;
	}
	_process.build_cgi_response();
	_fd = _client_fd;
	_event = POLLOUT;
	_func_ptr = &ClientSocket::send_response;

	return ;
}