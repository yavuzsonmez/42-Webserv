
#include "../../inc/request.hpp"

/* **********************************************************/

Request::Request() :
	_method.first(UNKNOWN), _method.second(false),
	_protocol.first("HTTP"), _protocol.second(true),
	_domain.first("/"), _domain.second(true),
	_port.first(80), _port.second(true),
	_scriptName.second(false),
	_path.second(false),
	_queryString.second(false),
	_fragment.second(false),
	_httpVersion.first("HTTP/1.1"), _httpVersion.second(true),
	_headers((NULL, false), (NULL, false)),
	_body.second(false)
	{}

Request::Request(std::string &req) :
	_method.first(UNKNOWN), _method.second(false),
	_protocol.first("HTTP"), _protocol.second(true),
	_domain.first("/"), _domain.second(true),
	_port.first(80), _port.second(true),
	_scriptName.second(false),
	_path.second(false),
	_queryString.second(false),
	_fragment.second(false),
	_httpVersion.first("HTTP/1.1"), _httpVersion.second(true),
	_headers((NULL, false), (NULL, false)),
	_body.second(false)
	{ parser(req); }

Request::Request( const Request &src ) :
	_method(src._method),
	_protocol(src._protocol),
	_port(src._port),
	_scriptName(src._scriptName),
	_path(src._path),
	_queryString(src._queryString),
	_fragment(src._fragment),
	_httpVersion(src._httpVersion),
	_headers(src._headers),
	_body(src._body) {}

Request::~Request() {}

/* **********************************************************/

std::pair<method, bool>			Request::getMethod() const { return _method; }
std::pair<std::string, bool>	Request::getProtocol() const { return _protocol; }
std::pair<std::string, bool>	Request::getDomain() const { return _domain; }
std::pair<unsigned int, bool>	Request::getPort() const { return _port; }
std::pair<std::string, bool>	Request::getScriptname() const { return _scriptName; }
std::pair<std::string, bool>	Request::getPath() const { return _path; }
std::pair<std::string, bool>	Request::getQuerystring() const { return _queryString; }
std::pair<std::string, bool>	Request::getFragment() const { return _fragment; }
std::pair<std::string, bool>	Request::getHttpversion() const { return _httpVersion; }
std::vector<std::pair<std::pair<std::string, bool>, std::pair<std::string, bool>>>
								Request::getHeaders() const { return _headers; }
std::pair<std::string, bool>	Request::getBody() const { return _body; }

/* **********************************************************/

void Request::parser(std::string &req)
{
	setMethod(req);
	setPath(req);
	setHttpversion(req);
}

void Request::setMethod(std::string &req)
{
	size_t pos = 0;
	std::string tmp;
	pos = req.find(" ");

	if (pos != std::string::npos)
		tmp = req.substr(0, pos);
	if (!tmp.compare("GET")) { _method.first = GET; _method.second = true; }
	else if (!tmp.compare("POST")) { _method.first = POST; _method.second = true; }
	else if (!tmp.compare("DELETE")) { _method.first = DELETE; _method.second = true; }
	else { _method.first = UNKNOWN; _method.second = false;}

	req.erase(0, pos + 1);
}

void Request::setPath(std::string &req)
{
	size_t pos = 0;
	pos = req.find(" ");

	_path.first = req.substr(0, pos);

	req.erase(0, pos + 1);
}

void Request::setHttpversion(std::string &req)
{
	size_t pos = 0;
	pos = req.find("\n");

	_httpVersion.first = req.substr(0, pos);
	if(!_httpVersion.first.compare("HTTP/1.1"))
		_httpVersion.second = true;
	else
		_httpVersion.second = false;
	req.erase(0, pos + 1);
}

/* **********************************************************/

std::ostream &			operator<<( std::ostream & o, Request const & i )
{

	typedef std::map<std::string, std::string>::const_iterator iterator;

	std::map<std::string, std::string> headers(i.getHeaders());
	o << R << "/* ************************************************************************** */" << std::endl << "/* "
	<< G << "method: " << i.getMethod() << std::endl
	<< R << "/* ************************************************************************** */"  << std::endl << "/* "
	<< B << "protocol: " << i.getProtocol() << std::endl << R << "/* "
	<< P << "domain: " << i.getDomain() << std::endl << R << "/* "
	<< Y << "port: " << i.getPort() << std::endl << R << "/* "
	<< G << "script name: " << i.getScriptname() << std::endl << R << "/* "
	<< B << "path: " << i.getPath() << std::endl << R << "/* "
	<< P << "query string: " << i.getQuerystring() << std::endl << R << "/* "
	<< Y << "fragment: " << i.getFragment() << std::endl
	<< R << "/* ************************************************************************** */" <<  std::endl << "/* "
	<< G << "http version: " << i.getHttpversion() << std::endl
	<< R << "/* ************************************************************************** */" <<  std::endl << "/* "
	<< B << "headers: " << std::endl;
	for(iterator it = headers.begin(); it != headers.end(); ++it)
		o << "\t\t" << it->first << " : " << it->second << std::endl << R << "/* ";
	o << R << "/* ************************************************************************** */" << std::endl << "/* "
	<< P << "body: " << std::endl
	<< i.getBody() << std::endl
	<< R << "/* ************************************************************************** */" << Reset << std::endl;
	return o;
}

/* **********************************************************/



	// std::string port;
	// std::stringstream ss;
	// ss << _port;
	// ss >> port;