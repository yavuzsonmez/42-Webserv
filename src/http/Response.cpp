
#include "../../inc/http/Response.hpp"


//Future Response class, just for testing the server right now
Response::Response()
{
	std::string body = get_file_content("../../sample/404.html");

	std::stringstream ss;
	ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: "
	<< body.length()
	<< "\n\n"
	<< body;

	_response = ss.str();
}

Response::~Response() {}

std::string Response::getResponse() const { return _response; }