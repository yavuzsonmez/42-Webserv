
#include "../../inc/request.hpp"

/* **********************************************************/

Request::Request() :
	_method(UNKNOWN, false),
	_protocol("HTTP", true),
	_domain("/", true),
	_port(80, true),
	_scriptName("", false),
	_path("", false),
	_queryString("", false),
	_fragment("", false),
	_httpVersion("HTTP/1.1", true),
	_headers(),
	_body("", false)
	{}

Request::Request(std::string &req) :
	_method(UNKNOWN, false),
	_protocol("HTTP", true),
	_domain("/", true),
	_port(80, true),
	_scriptName("", false),
	_path("", false),
	_queryString("", false),
	_fragment("", false),
	_httpVersion("HTTP/1.1", true),
	_headers(),
	_body("", false)
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

std::pair<method, bool>			Request::getMethod() const		{ return _method; }
std::pair<std::string, bool>	Request::getProtocol() const	{ return _protocol; }
std::pair<std::string, bool>	Request::getDomain() const		{ return _domain; }
std::pair<unsigned int, bool>	Request::getPort() const		{ return _port; }
std::pair<std::string, bool>	Request::getScriptname() const	{ return _scriptName; }
std::pair<std::string, bool>	Request::getPath() const		{ return _path; }
std::pair<std::string, bool>	Request::getQuerystring() const	{ return _queryString; }
std::pair<std::string, bool>	Request::getFragment() const	{ return _fragment; }
std::pair<std::string, bool>	Request::getHttpversion() const	{ return _httpVersion; }
vect_headrs_pairs				Request::getHeaders() const		{ return _headers; }
std::pair<std::string, bool>	Request::getBody() const		{ return _body; }

/* **********************************************************/

void Request::parser(std::string &req)
{
	setMethod(req);
	setPath(req);
	setHttpversion(req);

	setHeaders(req);
	setBody(req);
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

//support presence of ':' inside body
void Request::setHeaders(std::string &req)
{
	size_t pos = 0;
	std::pair<std::string, bool> hdr, direct;
	pos = req.find(":");
	while (pos != std::string::npos)
	{
		hdr = std::make_pair(req.substr(0, pos), true);
		req.erase(0, pos + 1);
		pos = req.find("\n");
		direct = std::make_pair(req.substr(0, pos), true);
		_headers.push_back(std::make_pair(hdr, direct));
		req.erase(0, pos + 1);
		pos = req.find(":");
	}
}

void Request::setBody(std::string &req)
{
	size_t pos = req.find("\n");
	_body = std::make_pair(req.substr(pos + 1), true);
	if (req.length())
		req.erase(pos);
}

/* **********************************************************/

std::ostream &			operator<<( std::ostream & o, Request const & i )
{

	std::pair<method, bool>			method = i.getMethod();
	std::string method_str;
	if (method.first == GET)
		method_str = "GET";
	else if (method.first == POST)
		method_str = "POST";
	else if (method.first == DELETE)
		method_str = "DELETE";
	else
		method_str = "UKNOWN";
	std::pair<std::string, bool>	protocol = i.getProtocol();
	std::pair<std::string, bool>	domain = i.getDomain();
	std::pair<unsigned int, bool>	port = i.getPort();
	std::pair<std::string, bool>	scriptName = i.getScriptname();
	std::pair<std::string, bool>	path = i.getPath();
	std::pair<std::string, bool>	queryString = i.getQuerystring();
	std::pair<std::string, bool>	fragment = i.getFragment();
	std::pair<std::string, bool>	httpVersion = i.getHttpversion();
	std::vector<std::pair<std::pair<std::string, bool>, std::pair<std::string, bool>>>
									headers = i.getHeaders();
	std::vector<std::pair<std::pair<std::string, bool>, std::pair<std::string, bool>>>::const_iterator it = headers.begin();
	std::vector<std::pair<std::pair<std::string, bool>, std::pair<std::string, bool>>>::const_iterator ite = headers.end();
	std::pair<std::string, bool>	body = i.getBody();

	o << P << "/* ************************************************************************** */" << std::endl << "/* "
	<< Y << "ELEMENT			SUPPORTED? ( 1 yes / 0 no)										"	<< std::endl
	<< P << "/* ************************************************************************** */" << std::endl << "/* "
	<< R << "method:\t\t" << B << method_str << "\t\t" << Reset << method.second << std::endl
	<< P << "/* ************************************************************************** */"  << std::endl << "/* "
	<< R << "protocol:\t\t" << B << protocol.first << "\t\t" << Reset << protocol.second << std::endl << P << "/* "
	<< R << "domain:\t\t" << B << domain.first << "\t\t" << Reset << domain.second << std::endl << P << "/* "
	<< R << "port:\t\t" << B << port.first << "\t\t" << Reset << port.second << std::endl << P << "/* "
	<< R << "script name:\t\t" << B << scriptName.first << Reset << "\t\t" << scriptName.second << std::endl << P << "/* "
	<< R << "path:\t\t" << B << path.first << "\t\t" << Reset << path.second << std::endl << P << "/* "
	<< R << "queryString:\t\t" << B << queryString.first << Reset << "\t\t" << queryString.second << std::endl << P << "/* "
	<< R << "fragment:\t\t" << B << fragment.first << "\t\t" << Reset << fragment.second << std::endl
	<< P << "/* ************************************************************************** */" <<  std::endl << "/* "
	<< R << "http version:\t" << B << httpVersion.first << Reset << "\t" << httpVersion.second << std::endl
	<< P << "/* ************************************************************************** */" <<  std::endl << "/* "
	<< R << "headers:" << std::endl;
	for (; it != ite; ++it)
	{
		o << B << (*it).first.first << Reset << " " << (*it).first.second << " : "
		<< B << (*it).second.first << Reset << " " << (*it).second.second << std::endl;
	}
	o << P << "/* ************************************************************************** */" <<  std::endl << "/* "
	<< R << "body:\t\t" << B << body.first << Reset << "\t\t" << body.second << std::endl
	<< P << "/* ************************************************************************** */" <<  std::endl;
	return o;
}