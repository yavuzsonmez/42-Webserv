
#include "../../inc/request.hpp"


// Constructors

Request::Request() :
	_method(UNKNOWN, false),
	_protocol("http", true),
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
	_protocol("http", true),
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

// Getters

ui_flag				Request::getMethod() const		{ return _method; }
str_flag			Request::getProtocol() const	{ return _protocol; }
str_flag			Request::getDomain() const		{ return _domain; }
ui_flag				Request::getPort() const		{ return _port; }
str_flag			Request::getScriptname() const	{ return _scriptName; }
str_flag			Request::getPath() const		{ return _path; }
str_flag			Request::getQuerystring() const	{ return _queryString; }
str_flag			Request::getFragment() const	{ return _fragment; }
str_flag			Request::getHttpversion() const	{ return _httpVersion; }
headr_dirctiv		Request::getHeaders() const		{ return _headers; }
str_flag			Request::getBody() const		{ return _body; }

// Parsing

void Request::parser(std::string &req) {
	setMethod(req);
	setUrl(req);
	setHttpversion(req);
	setHeaders(req);
	setBody(req);
}

// method
void Request::setMethod(std::string &req) {
	std::string tmp;
	size_t pos = req.find(" ");
	if (pos != std::string::npos)
		tmp = req.substr(0, pos);
	if (!tmp.compare("GET"))			{ _method.first = GET; _method.second = true; }
	else if (!tmp.compare("POST"))		{ _method.first = POST; _method.second = true; }
	else if (!tmp.compare("DELETE"))	{ _method.first = DELETE; _method.second = true; }
	else								{ _method.first = UNKNOWN; _method.second = false;}

	req.erase(0, pos + 1);
}

// TODO check std::string::npos before substr qnd erase

// URL
void Request::setUrl(std::string &req) {
	size_t pos = req.find(" ");
	std::string url = req.substr(0, pos);
	req.erase(0, pos + 1);

	setProtocol(url);

	//setDomain(url);
	//setPort(url);
	//setScriptname(url);
	//setPath(url);
}

void Request::setProtocol(std::string &url) {

	size_t pos = url.find("://");
	if (pos == std::string::npos)
		return ;
	_protocol.first = url.substr(0, pos);
	if (_protocol.first.compare("http"))
		_protocol.second = false;
	url.erase(0, pos + 1);
}

void Request::setDomain(std::string &url) { return; }
void Request::setPort(std::string &url) { return; }
void Request::setScriptname(std::string &url) { return; }

void Request::setPath(std::string &url)
{
/* 	size_t pos = url.find(" ");

	_path.first = url.substr(0, pos);

	url.erase(0, pos + 1); */
}

void Request::setQuerystring(std::string &url) { return; }
void Request::setFragment(std::string &url) { return; }

// HTTP version
void Request::setHttpversion(std::string &req)
{
	size_t pos = req.find("\n");

	_httpVersion.first = req.substr(0, pos);
	if(!_httpVersion.first.compare("HTTP/1.1"))
		_httpVersion.second = true;
	else
		_httpVersion.second = false;
	req.erase(0, pos + 1);
}

// TODO support presence of ':' inside body
// Headers
void Request::setHeaders(std::string &req)
{
	str_flag hdr, direct;
	size_t pos = req.find(":");
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

// Body
void Request::setBody(std::string &req)
{
	size_t pos = req.find("\n");
	_body = std::make_pair(req.substr(pos + 1), true);
	if (req.length())
		req.erase(pos);
}

// Overlaod to print the Request

std::ostream &			operator<<( std::ostream & o, Request const & i )
{

	ui_flag method = i.getMethod();
	std::string method_str;
	if (method.first == GET)
		method_str = "GET";
	else if (method.first == POST)
		method_str = "POST";
	else if (method.first == DELETE)
		method_str = "DELETE";
	else
		method_str = "UKNOWN";
	str_flag		protocol = i.getProtocol();
	str_flag		domain = i.getDomain();
	ui_flag		port = i.getPort();
	str_flag		scriptName = i.getScriptname();
	str_flag		path = i.getPath();
	str_flag		queryString = i.getQuerystring();
	str_flag		fragment = i.getFragment();
	str_flag		httpVersion = i.getHttpversion();
	headr_dirctiv	headers = i.getHeaders();
		headr_dirctiv::const_iterator it = headers.begin();
		headr_dirctiv::const_iterator ite = headers.end();
	str_flag	body = i.getBody();

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