
#include "../../inc/request.hpp"

/* **********************************************************/

Request::Request() :
	_method(UNKNOWN),
	_protocol(HTTP),
	_domain("/"),
	_port(80),
	_httpVersion("HTTP/1.1") {}

Request::Request(std::string &req) :
	_method(UNKNOWN),
	_protocol(HTTP),
	_domain("/"),
	_port(80),
	_httpVersion("HTTP/1.1") { parser(req); }

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

std::string Request::getMethod() const {
	if (_method == GET)
		return ("GET");
	if (_method == POST)
		return ("POST");
	if (_method == DELETE)
		return ("DELETE");
	return ("UNKNOWN");
}

std::string	Request::getProtocol() const {
	if (_protocol == HTTP)
		return ("HTTP");
	return ("UNKNOWN");
}

std::string Request::getDomain() const { return _domain; }

std::string Request::getPort() const {
	std::string port;
	std::stringstream ss;
	ss << _port;
	ss >> port;
	return port;
}

std::string Request::getScriptname() const { return _scriptName; }

std::string Request::getPath() const { return _path; }

std::string Request::getQuerystring() const { return _queryString; }

std::string Request::getFragment() const { return _fragment; }

std::string Request::getHttpversion() const { return _httpVersion; }

std::map <std::string, std::string> Request::getHeaders() const {
	return _headers;
}

std::string Request::getBody() const { return _body; }

/* **********************************************************/

void Request::parser(std::string &req)
{
	setMethod(req);
	setPath(req);
}

void Request::setMethod(std::string &req)
{
	size_t pos = 0;
	std::string tmp;

	pos = req.find(" ");
	if (pos != std::string::npos)
		tmp = req.substr(0, pos);
	if (!tmp.compare("GET"))
		_method = GET;
	else if (!tmp.compare("POST"))
		_method = POST;
	else if (!tmp.compare("DELETE"))
		_method = DELETE;
	else
		_method = UNKNOWN;
	req.erase(0, pos + 1);
}

void Request::setPath(std::string &req)
{
	size_t pos = 0;

	pos = req.find(" ");
	_path = req.substr(0, pos);
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