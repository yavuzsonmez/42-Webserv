
#include "../../inc/http/Request.hpp"


/**
 * @brief Default constructor, default values for testing
 */
Request::Request() :
	_method(UNKNOWN, false),
	_protocol("http", true),
	_domain("", true),
	_port(80, true),
	_script("", false),
	_path("/", true),
	_query("", false),
	_fragment("", false),
	_httpVersion("HTTP/1.1", true),
	_headers(),
	_body("", false),
	_status("")
	{}

/**
 * @brief Prefered constructor, default values
 * @param req, raw request sent by the client
 */
Request::Request(std::string &req) :
	_method(UNKNOWN, false),
	_protocol("http", true),
	_domain("", true),
	_port(80, true),
	_script("", false),
	_path("/", true),
	_query("", false),
	_fragment("", false),
	_httpVersion("HTTP/1.1", true),
	_headers(),
	_body("", false),
	_status("")
	{ parser(req); }

/**
 * @brief Copy constructor
 */
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

/**
 * @brief Getters functions, read only
 */
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

/**
 * @brief Main parsing function, each subfunction will
 * parse and store a specific element of the request and erase it
 * in the request string.
 */
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

/**
 * @brief Check the method in the request, and verify if supported
 */
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
// TODO domain is case insensitive

/**
 * @brief Separate the different element in the URL of the request
 */
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
	setScript(url);
	setPath(url);
}

/**
 * @brief Check if the protocol is described in the request
 * if yes, check if we support it (only http)
 */
void Request::setProtocol(std::string &url) {

	size_t pos = url.find("://");
	if (pos == std::string::npos)
		return ;
	_protocol.first = url.substr(0, pos);
	if (_protocol.first.compare("http"))
		_protocol.second = false;
	url.erase(0, pos + 3);
}

/**
 * @brief Store the domain name of the request
 */
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

/**
 * @brief Check if a port is provided in the URL of the request,
 * if not use the default http port (80)
 */
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

/**
 * @brief check if client requests a CGI
 */
void Request::setScript(std::string &url) {
	size_t pos = url.find_last_of("/");
	if (pos == std::string::npos)
		return ;
	_script.first = url.substr(0, pos);
	if (_script.first.substr(0, _script.first.find("/")).compare("cgi")) // check if cgi the client request the CGI in the cgi/ folder
		_script.second = false;
	else
		_script.second = true;
	url.erase(0, pos + 1);
	// TODO check if URL containes something after script like /cgi/index.php/ or /cgi/index.php
	// fix research based on last / for script
}


/**
 * @brief
 */
void Request::setPath(std::string &url)
{
	setFragment(url);
	setQuery(url);

	if (url.length())
		_path.first = url.substr(0, url.length());
	url.erase(0, url.length());
}

/**
 * @brief
 */
void Request::setQuery(std::string &url) {
	size_t pos = url.find("?");
	if (pos != std::string::npos)
	{
		_query.first = url.substr(pos + 1, url.length());
		url.erase(pos, url.length());
		_query.second = true;
	}
}

/**
 * @brief
 */
void Request::setFragment(std::string &url) {
	size_t pos = url.find("#");
	if (pos != std::string::npos)
	{
		_fragment.first = url.substr(pos + 1, url.length());
		url.erase(pos, url.length());
		_fragment.second = true;
	}
}

/**
 * @brief Check the HTTP version used in the request. Webserv support only HTTP/1.1
 */
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
/**
 * @brief Store every single header in a vector of pair
 * vector -> <header: directive>
 * 			header -> <header, flag>
 *			directive -> <directive, flag>
 */
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

/**
 * @brief Store the eventual body (POST request) of the request
 */
void Request::setBody(std::string &req)
{
	size_t pos = req.find("\n");
	_body = std::make_pair(req.substr(pos + 1), true);
	if (req.length())
		req.erase(pos);
}

/**
 * @brief Overload that print all information of the parsed request
 *	for debugging
 */
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
	<< R << "script:\t" << B << script.first << Reset << "\t\t\t\t" << script.second << std::endl << P << "/* "
	<< R << "path:\t" << B << path.first << "\t\t\t\t" << Reset << path.second << std::endl << P << "/* "
	<< R << "query:\t" << B << query.first << Reset << "\t\t\t\t" << query.second << std::endl << P << "/* "
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