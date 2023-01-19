# include "../../inc/http/Response.hpp"

Response::Response(void)
{

}

Response::Response(Request request, ServerBlock config) : _request(request), _config(config)
{

}

Response::~Response(void)
{

}

void	Response::set_protocol(std::string protocol){_protocol = protocol;}
void	Response::set_status_code(std::string status_code){_status_code = status_code;}
void	Response::set_status_text(std::string status_text){_status_text = status_text;}
void	Response::set_server(std::string server){_server = server;}
void	Response::set_redirection(std::string redirection){_redirection = redirection;}
void	Response::set_content_type(std::string content_type){_content_type = content_type;}
void	Response::set_charset(std::string charset){_charset = charset;}
void	Response::set_content_length(std::string content_length){_content_length = content_length;}
void	Response::set_transfer_encoding(std::string transfer_encoding){_transfer_encoding = transfer_encoding;}
void	Response::set_body(std::string body){_body = body;}

std::string	Response::get_protocol(void){return _protocol;}
std::string	Response::get_status_code(void){return _status_code;}
std::string	Response::get_status_text(void){return _status_text;}
std::string	Response::get_server(void){return _server;}
std::string	Response::get_redirection(void){return _redirection;}
std::string	Response::get_content_type(void){return _content_type;}
std::string	Response::get_charset(void){return _charset;}
std::string	Response::get_content_length(void){return _content_length;}
std::string	Response::get_transfer_encoding(void){return _transfer_encoding;}
std::string	Response::get_body(void){return _body;}

std::string	&Response::get_response(void)
{
	return _response;
}

void	Response::create_response(void)
{
	_raw_body = _body;
	// parse the headers from the _raw_body
	size_t pos = _raw_body.find("\r\n\r\n");
	if (pos != std::string::npos) {
		_headers_raw = _raw_body.substr(0, pos);
		_raw_body = _raw_body.substr(pos + 4);
	}
	_response = _protocol + " " + _status_code + " " + _status_text + "\r\n";
	if (!_redirection.empty())
		_response += "location: " + _redirection + "\r\n";
	if (!_content_type.empty() && _headers_raw.find("Content-type") == std::string::npos)
		_response += "content-type: " + _content_type + "\r\n";
	if (!_content_length.empty())
		_response += "content-length: " + _content_length + "\r\n";
	if (!_headers_raw.empty())
		_response += _headers_raw + "\r\n";
	_response += "webserver: PETROULETTE\r\n";
	if (!_body.empty())
		_response += "\r\n" + _body;
	// _raw_body = _body;
	// size_t pos = _raw_body.find("\r\n\r\n");
	// if (pos != std::string::npos) {
	// 	_headers_raw = _raw_body.substr(0, pos);
	// 	_raw_body = _raw_body.substr(pos + 4);
	// }
	// _response = _protocol + " " + _status_code + " " + _status_text + "\r\n";
	// //_response += "server: " + _server + "\r\n";
	// if (!_redirection.empty())
	// 	_response += "Location: " + _redirection + "\r\n";
	// if (!_content_type.empty())
	// 	_response += "content-type: " + _content_type + "\r\n";
	// if (!_content_length.empty())
	// 	_response += "content-length: " + to_str(_raw_body.size()) + "\r\n";
	// _response += "webserver: PETROULETTE\r\n";
	// if (!_headers_raw.empty())
	// 	_response += _headers_raw + "\r\n";
	// if (!_body.empty())
	// 	_response += "\r\n\r\n" + _raw_body;
}

std::string	Response::get_file_format(void)
{
	if (_body.find("html") != (unsigned long) -1)
		return ("text/html");
	if (_body.find("PNG") != (unsigned long) -1)
		return ("image/png");
	if (_body.find("JFIF") != (unsigned long) -1)
		return ("image/jpeg");
	if (_body.find("GIF") != (unsigned long)-1)
		return ("image/gif");
	if (_body.find("MPEG-4") != (unsigned long)-1)
		return ("video/mp4");
	return("unknown");
}