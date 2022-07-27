#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>
# include <vector>
# include <sstream>
# include "colors.hpp"

enum method { GET, POST, DELETE, UNKNOWN };

class Request
{

	public:

		Request();
		Request( std::string &req );
		Request( const Request &src );
		~Request();

		//getters
		std::pair<method, bool>					getMethod() const;
		std::pair<std::string, bool>			getProtocol() const;
		std::pair<std::string, bool>			getDomain() const;
		std::pair<unsigned int, bool>			getPort() const;
		std::pair<std::string, bool>			getScriptname() const;
		std::pair<std::string, bool>			getPath() const;
		std::pair<std::string, bool>			getQuerystring() const;
		std::pair<std::string, bool>			getFragment() const;
		std::pair<std::string, bool>			getHttpversion() const;
		std::vector<std::pair<std::pair<std::string, bool>, std::pair<std::string, bool>>>
												getHeaders() const;
		std::pair<std::string, bool>			getBody() const;

		//main parser
		void	parser(std::string &req);
		//setters for parser
		void	setMethod(std::string &req);
		void	setProtocol(std::string &req);
		void	setDomain(std::string &req);
		void	setPort(std::string &req);
		void	setScriptname(std::string &req);
		void	setPath(std::string &req);
		void	setQuerystring(std::string &req);
		void	setFragment(std::string &req);
		void	setHttpversion(std::string &req);
		void	setHeaders(std::string &req);
		void	setBody(std::string &req);

	private:
		std::pair<method, bool>				_method;
		std::pair<std::string, bool>		_protocol;
		std::pair<std::string, bool>		_domain;
		std::pair<unsigned int, bool>		_port;
		std::pair<std::string, bool>		_scriptName;
		std::pair<std::string, bool>		_path;
		std::pair<std::string, bool>		_queryString;
		std::pair<std::string, bool>		_fragment;
		std::pair<std::string, bool>		_httpVersion;
		std::vector<std::pair<std::pair<std::string, bool>, std::pair<std::string, bool>>>
											_headers;
		std::pair<std::string, bool>		_body;
};

std::ostream &			operator<<( std::ostream & o, Request const & i );

#endif /* ********************************************************* REQUEST_H */
