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

// void	Response::process_request(void)
// {
// 	std::cout << "request: " << _request.getScript().first << std::endl;
// 	_vec_loc = _config.getConfigurationKeysWithType(LOCATION);
// 	if (_request.getScript().first.empty())
// 	{
// 		_location = Location(_config);
// 	}
// 	else
// 	{
// 		int flag = 0;
// 		for (std::vector<ConfigurationKey>::iterator it = _vec_loc.begin(); it != _vec_loc.end(), it++)
// 		{
// 			if (_request.getScript().first == (*it).location);
// 			{
// 				_location = Location(*it);
// 				flag = 1;
// 				break;
// 			}
// 		}
// 		if (!flag)
// 		{
// 			_location = Location();
// 		}
// 	}
	
// 	if(_location._index.substr(fn.find_last_of(".") + 1) == "conf")
// }

void	Response::set_protocol(std::string protocol){_protocol = protocol;}
void	Response::set_status_code(std::string status_code){_status_code = status_code;}
void	Response::set_status_text(std::string status_text){_status_text = status_text;}
void	Response::set_server(std::string server){_server = server;}
void	Response::set_content_type(std::string content_type){_content_type = content_type;}
void	Response::set_charset(std::string charset){_charset = charset;}
void	Response::set_content_length(std::string content_length){_content_length = content_length;}
void	Response::set_transfer_encoding(std::string transfer_encoding){_transfer_encoding = transfer_encoding;}
void	Response::set_body(std::string body){_body = body;}

std::string	Response::get_protocol(void){return _protocol;}
std::string	Response::get_status_code(void){return _status_code;}
std::string	Response::get_status_text(void){return _status_text;}
std::string	Response::get_server(void){return _server;}
std::string	Response::get_content_type(void){return _content_type;}
std::string	Response::get_charset(void){return _charset;}
std::string	Response::get_content_length(void){return _content_length;}
std::string	Response::get_transfer_encoding(void){return _transfer_encoding;}
std::string	Response::get_body(void){return _body;}

std::string	Response::get_response(void)
{
	return _response;
}

void	Response::create_response(void)
{
	_response = _protocol + " " + _status_code + " " + _status_text + "\r\n";
	_response += "server: " + _server + "\r\n";
	if (!_content_type.empty())
		_response += "content-type: " + _content_type + "\r\n";
	if (!_content_length.empty())
		_response += "content-length: " + _content_length + "\r\n";
	// if (!_html.empty())
	// 	_response += "\r\n" + _html;
	// if (!_plain.empty())
	// 	_response += "\r\n" + _plain;
	// if (!_image.empty())
	// 	_response += "\r\n" + _image;
	// if (!_transfer_encoding.empty())
	// {
	// 	_response += "transfer-encoding: " + _transfer_encoding + "\r\n\r\n";
	// 	return ;
	// }
	if (!_body.empty())
		_response += "\r\n" + _body;
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

void	Response::test_text(void)
{
	_protocol = "HTTP/1.1";
	_status_code = "200";
	_status_text = "OK";

	_server = "PetRoulette";
	_content_type = "text/plain";
	_body = "Hello World";
	_content_length = to_str(_body.length());
	create_response();
}

void	Response::test_html(void)
{
	_protocol = "HTTP/1.1";
	_status_code = "200";
	_status_text = "OK";

	_server = "PetRoulette";
	_content_type = "text/html";
	_body = get_file_content("html/index.html");
	_content_length = to_str(_body.length());
	create_response();
}

void	Response::test_image(void)
{
	_protocol = "HTTP/1.1";
	_status_code = "200";
	_status_text = "OK";

	_server = "PetRoulette";
	//_content_type = "image/png";
	_body = get_file_content("images/Rotating_earth_(large).gif");
	_content_type = get_file_format();
	_content_length = to_str(_body.length());

	create_response();
}

// void	Response::test_cgi(ServerBlock &config)
// {
// 	_protocol = "HTTP/1.1";
// 	_status_code = "200";
// 	_status_text = "OK";
// 	CGI	cgi(_request, config);
// 	cgi.execute();
// 	_body = cgi.get_buf();
// 	_server = "PetRoulette";
// 	_content_type = "text/html";
// 	_content_length = to_str(_body.length());
// 	create_response();
// }
