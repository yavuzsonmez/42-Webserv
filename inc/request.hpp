#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>
# include <map>
# include <sstream>
# include "colors.hpp"

enum method { GET, POST, DELETE, UNKNOWN };
enum protocol { HTTP, OTHER };

class Request
{

	public:

		Request();
		Request( std::string const &req );
		Request( Request const & src );
		~Request();

		//main parser
		void parser(std::string const &req);

		//getters
		std::string getMethod() const;
		std::string	getProtocol() const;
		std::string getDomain() const;
		std::string getPort() const;
		std::string getScriptname() const;
		std::string getPath() const;
		std::string getQuerystring() const;
		std::string getFragment() const;
		std::string getHttpversion() const;
		std::map <std::string, std::string> getHeaders() const;
		std::string getBody() const;

	private:

		// Method
		unsigned short	const _method; //method

		// Url
		unsigned short	const _protocol;
		std::string		const _domain;
		unsigned int	const _port;
		std::string		const _scriptName;
		std::string		const _path;
		std::string		const _queryString;
		std::string		const _fragment;

		// Http version
		std::string		const _httpVersion;

		// Headers
		std::map <std::string, std::string>	const _headers;

		// Body
		std::string		const _body;
};

std::ostream &			operator<<( std::ostream & o, Request const & i );

#endif /* ********************************************************* REQUEST_H */