#include "../../inc/network/ClientSocket.hpp"
#include "../../inc/debugger/DebuggerPrinter.hpp"



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
}

ClientSocket::~ClientSocket()
{

}

bool ClientSocket::Timeout()
{
	if (std::time(NULL) - _timeout > 5)
		return true;
	return false;

}

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
	_bytes = read(_fd, (char*)buffer.data() + _position, _count);
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
 * @brief writes the the response to the clientSocket
 */
void	ClientSocket::write_from_buffer(void)
{
	_bytes = send(_fd, _process._response.get_response().data() + _position, _process._response.get_response().length(), 0);
	_position += _bytes;
	if (_position >= _process._response.get_response().length())
	{
		close(_fd);
		_remove = true;
		return ;
	}
	return ;
}

/**
 * @brief sets up the process object.
 * in case of an cgi, it sets the _func_ptr to the next function that handels the cgi and updates the filedescriptor
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
	if (_process._with_cgi)
	{
		_bytes = 0;
		_position = 0;
		_func_ptr = &ClientSocket::one;
		_fd = _process._CGI._fd_in;
		_event = POLLOUT;
	}
	else
	{
		_bytes = 0;
		_position = 0;
		_func_ptr = &ClientSocket::write_from_buffer;
		_event = POLLOUT;
	}
	return ;
}

/**
 * @brief executes the function related to the filedescriptor and prepare for the next one
 */
void	ClientSocket::one(void)
{
	_process._CGI.write_in_std_in();
	_fd = _process._CGI._fd_out;
	_event = POLLOUT;
	_func_ptr = &ClientSocket::two;
	return ;
}

void	ClientSocket::two(void)
{
	USE_DEBUGGER;
	try {
		_process._CGI.write_in_std_out();
	} catch (int error) {
		debugger.error("Error in CGI");
		return ;
	}
	_fd = _process._CGI._fd_out;
	_event = POLLIN;
	_func_ptr = &ClientSocket::three;
	return ;
}

void	ClientSocket::three(void)
{
	_process._CGI.read_in_buff();
	_process.build_cgi_response();
	_fd = _client_fd;
	_event = POLLOUT;
	_func_ptr = &ClientSocket::write_from_buffer;
	return ;
}