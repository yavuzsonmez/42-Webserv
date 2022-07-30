# include "../../inc/response.hpp"

response::response(void)
{

}

response::~response(void)
{

}

std::string	response::get_response(void)
{
	return _response;
}

void	response::create_response(void)
{
	_response = _protocol + " " + _status_code + " " + _status_text + "\n";
	_response += "server: " + _server + "\n";
	if (!_content_type.empty())
		_response += "content-type: " + _content_type + "\n";
	if (!_content_length.empty())
		_response += "content-length: " + _content_length + "\n";
	if (!_html.empty())
		_response += "\n" + _html;
	if (!_plain.empty())
		_response += "\n" + _plain;
}

void	response::test_text(void)
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

void	response::test_html(void)
{
	_protocol = "HTTP/1.1";
	_status_code = "200";
	_status_text = "OK";

	_server = "PetRoulette";
	_content_type = "text/html";
	read_html("html/index.html");
	create_response();
}

void	response::test_image(void)
{
	_protocol = "HTTP/1.1";
	_status_code = "200";
	_status_text = "OK";

	_server = "PetRoulette";
	_content_type = "image/png";
	read_html("images/petroulette.png");
	create_response();
}

void	response::read_html(std::string path)
{
	_html = get_file_content(path);
	_content_length = to_str(_html.length());
}