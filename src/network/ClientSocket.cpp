#include "../../inc/network/ClientSocket.hpp"



ClientSocket::ClientSocket(struct sockaddr_in clientSocket, ServerBlock config, int forward)
{
	//if (!_socket)
	// fail exit

	_socket.sin_family = clientSocket.sin_family;
	_socket.sin_port = clientSocket.sin_port;
	_socket.sin_addr.s_addr = clientSocket.sin_addr.s_addr;
	bzero(&(_socket.sin_zero), 8);

	_config = config;

	_state = HEADER;
	_fd = forward;
	_position = 0;
	_count = 32000;
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

int	ClientSocket::read_in_buffer(void)
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
				return 0;
			}
		}
	}
	else if (_state == BODY)
	{
		if (_position == _content_length)
		{
			set_up();
			return 0;
		}
		//process
	}
	//if (_bytes == 0 && Timeout())
	//	throw (Request_Timeout);
	//else if (_bytes == -1)
	//	throw (Internal_Server_Error);
	return 1;
}

int	ClientSocket::write_from_buffer(void)
{
	_bytes = send(_fd, _response_buffer.data() + position, _response_buffer.length(), 0);
	_positiom += bytes;
	if (_position >= _response_buffer.length())
	{
		close(_fd);
		return 0
	}
	return 1;
}

int	ClientSocket::set_up(void)
{
	_process = Process(_response, _clientRequest, _config);
	try
	{
		_process.process_request();
	}
	catch (int e)
	{
		_process.exception(e);
	}
	std::string _response_buffer(_response.get_response());
	_state = RESPONSE;
	_bytes = 0;
	_position = 0;
	return 1;
}