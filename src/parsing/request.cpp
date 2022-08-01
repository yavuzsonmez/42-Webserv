
#include "../../inc/Request.hpp"


// Constructors

Request::Request() :
	_method(UNKNOWN, false),
	_protocol("http", true),
	_domain("/", true),
	_port(80, true),
	_script("", false),
	_path("", false),
	_query("", false),
	_fragment("", false),
	_httpVersion("HTTP/1.1", true),
	_headers(),
	_body("", false),
	_status("")
	{}

Request::Request(std::string &req) :
	_method(UNKNOWN, false),
	_protocol("http", true),
	_domain("/", true),
	_port(80, true),
	_script("", false),
	_path("", false),
	_query("", false),
	_fragment("", false),
	_httpVersion("HTTP/1.1", true),
	_headers(),
	_body("", false),
	_status("")
	{ parser(req); }

Request::Request( const Request &src ) :
	_method(src._method),
	_protocol(src._protocol),
	_port(src._port),
	_script(src._script),
	_path(src._path),
	_query(src._query),
	_fragment(src._fragment),
	_httpVersion(src._httpVersion),
	_headers(src._headers),
	_body(src._body) {}

Request::~Request() {}

// Getters

i_flag				Request::getMethod() const		{ return _method; }
str_flag			Request::getProtocol() const	{ return _protocol; }
str_flag			Request::getDomain() const		{ return _domain; }
i_flag				Request::getPort() const		{ return _port; }
str_flag			Request::getScript() const		{ return _script; }
str_flag			Request::getPath() const		{ return _path; }
str_flag			Request::getQuery() const		{ return _query; }
str_flag			Request::getFragment() const	{ return _fragment; }
str_flag			Request::getHttpversion() const	{ return _httpVersion; }
headr_dirctiv		Request::getHeaders() const		{ return _headers; }
str_flag			Request::getBody() const		{ return _body; }
std::string			Request::getStatus() const		{ return _status; }

// Parsing

void Request::parser(std::string &req) {
	if (req.empty())
	{
		_status = Bad_Request;
		return ;
	}
	setMethod(req);
	if (!_status.empty())
		return ;
	setUrl(req);
	setHttpversion(req);
	setHeaders(req);
	setBody(req);
}

// method
void Request::setMethod(std::string &req) {
	std::string tmp;
	size_t pos = req.find(" ");
	if (pos == std::string::npos)
	{
		_status = Bad_Request;
		return ;
	}
	tmp = req.substr(0, pos);
	if (!tmp.compare("GET"))			{ _method.first = GET; _method.second = true; }
	else if (!tmp.compare("POST"))		{ _method.first = POST; _method.second = true; }
	else if (!tmp.compare("DELETE"))	{ _method.first = DELETE; _method.second = true; }
	else { _method.first = UNKNOWN; _method.second = false; _status =  Method_Not_Allowed; }
	req.erase(0, pos + 1);
}

// TODO check std::string::npos before substr qnd erase

// URL
void Request::setUrl(std::string &req) {
	size_t pos = req.find(" ");
	if (pos == std::string::npos)
	{
		_status = Bad_Request;
		return ;
	}
	std::string url = req.substr(0, pos);
	req.erase(0, pos + 1);
	setProtocol(url);
	setDomain(url);
	//setScript(url);
	//setPath(url);
	//setQuery(url);
}

void Request::setProtocol(std::string &url) {

	size_t pos = url.find("://");
	if (pos == std::string::npos)
		return ;
	_protocol.first = url.substr(0, pos);
	if (_protocol.first.compare("http"))
		_protocol.second = false;
	url.erase(0, pos + 3);
}

void Request::setDomain(std::string &url) {
	setPort(url);
	size_t pos = url.find("/");
	if (pos == std::string::npos)
	{
		_domain.first.clear();
		_domain.second = false;
		_status = Bad_Request;
		return ;
	}
	_domain.first = url.substr(0, pos);
	url.erase(0, pos + 1);
}

void Request::setPort(std::string &url) {
	size_t x = url.find(":");
	size_t y = url.find("/");
	if (x == std::string::npos || y == std::string::npos)
		return;
	std::string port = url.substr(x + 1, y - x - 1);
	std::stringstream ss(port);
	ss >> _port.first;
	if (_port.first < 0)
	{
		_status = Bad_Request;
		_port.second = false;
	}
	url.erase(x, y - x );
}

void Request::setScript(std::string &url) { return; }

void Request::setPath(std::string &url)
{
/* 	size_t pos = url.find(" ");

	_path.first = url.substr(0, pos);

	url.erase(0, pos + 1); */
}

void Request::setQuery(std::string &url) { return; }
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

	i_flag method = i.getMethod();
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
	i_flag			port = i.getPort();
	str_flag		script = i.getScript();
	str_flag		path = i.getPath();
	str_flag		query = i.getQuery();
	str_flag		fragment = i.getFragment();
	str_flag		httpVersion = i.getHttpversion();
	headr_dirctiv	headers = i.getHeaders();
		headr_dirctiv::const_iterator it = headers.begin();
		headr_dirctiv::const_iterator ite = headers.end();
	str_flag	body = i.getBody();

	o << P << "/* ************************************************************************** */" << std::endl << "/* "
	<< Y << "ELEMENT			SUPPORTED? ( 1 yes / 0 no)										"	<< std::endl
	<< P << "/* ************************************************************************** */" << std::endl << "/* "
	<< R << "method:\t" << B << method_str << "\t\t\t\t" << Reset << method.second << std::endl
	<< P << "/* ************************************************************************** */"  << std::endl << "/* "
	<< R << "protocol:\t" << B << protocol.first << "\t\t\t\t" << Reset << protocol.second << std::endl << P << "/* "
	<< R << "domain:\t" << B << domain.first << "\t\t" << Reset << domain.second << std::endl << P << "/* "
	<< R << "port:\t" << B << port.first << "\t\t\t\t" << Reset << port.second << std::endl << P << "/* "
	<< R << "script name:\t" << B << script.first << Reset << "\t\t\t\t" << script.second << std::endl << P << "/* "
	<< R << "path:\t" << B << path.first << "\t\t\t\t" << Reset << path.second << std::endl << P << "/* "
	<< R << "queryString:\t" << B << query.first << Reset << "\t\t\t\t" << query.second << std::endl << P << "/* "
	<< R << "fragment:\t" << B << fragment.first << "\t\t\t\t" << Reset << fragment.second << std::endl
	<< P << "/* ************************************************************************** */" <<  std::endl << "/* "
	<< R << "http version:\t" << B << httpVersion.first << Reset << "\t\t" << httpVersion.second << std::endl
	<< P << "/* ************************************************************************** */" <<  std::endl << "/* "
	<< R << "headers:" << std::endl;
	for (; it != ite; ++it)
	{
		o << B << (*it).first.first << Reset << " " << (*it).first.second << " : "
		<< B << (*it).second.first << Reset << " " << (*it).second.second << std::endl;
	}
	o << P << "/* ************************************************************************** */" <<  std::endl << "/* "
	<< R << "body:\t" << B << body.first << Reset << "\t\t\t\t" << body.second << std::endl
	<< P << "/* ************************************************************************** */" <<  std::endl
	<< G << i.getStatus() << Reset << std::endl;
	return o;
}