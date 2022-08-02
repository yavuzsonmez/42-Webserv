# include "../../inc/Response.hpp"

Response::Response(void)
{

}

Response::~Response(void)
{

}

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
	if (!_html.empty())
		_response += "\r\n" + _html;
	if (!_plain.empty())
		_response += "\r\n" + _plain;
	if (!_image.empty())
		_response += "\r\n" + _image;
}

std::string	Response::get_image_format(void)
{
	if (_image.find("PNG") != -1)
		return ("image/png");
	if (_image.find("JFIF") != -1)
		return ("image/jpeg");
	if (_image.find("GIF") != -1)
		return ("image/gif");
	return("no valid format");
}

void	Response::test_text(void)
{
	_protocol = "HTTP/1.1";
	_status_code = "200";
	_status_text = "OK";

	_server = "PetRoulette";
	_content_type = "text/plain";
	_plain = "Hello World";
	_content_length = to_str(_plain.length());
	create_response();
}

void	Response::test_html(void)
{
	_protocol = "HTTP/1.1";
	_status_code = "200";
	_status_text = "OK";

	_server = "PetRoulette";
	_content_type = "text/html";
	_html = get_file_content("html/index.html");
	_content_length = to_str(_html.length());
	create_response();
}

void	Response::test_image(void)
{
	_protocol = "HTTP/1.1";
	_status_code = "200";
	_status_text = "OK";

	_server = "PetRoulette";
	//_content_type = "image/png";
	_image = get_file_content("images/Rotating_earth_(large).gif");
	_content_type = get_image_format();
	_content_length = to_str(_image.length());

	create_response();
}
