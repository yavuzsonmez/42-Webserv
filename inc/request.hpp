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
		Request( std::string &req );
		Request( const Request &src );
		~Request();

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

		//main parser
		void parser(std::string &req);
		//setters for parser
		void setMethod(std::string &req);
		void setProtocol(std::string &req);
		void setDomain(std::string &req);
		void setPort(std::string &req);
		void setScriptname(std::string &req);
		void setPath(std::string &req);
		void setQuerystring(std::string &req);
		void setFragment(std::string &req);
		void setHttpversion(std::string &req);
		void setHeaders(std::string &req);
		void setBody(std::string &req);

	private:

		// Method
		unsigned short	 _method; //method
		// Url
		unsigned short	 _protocol;
		std::string		 _domain;
		unsigned int	 _port;
		std::string		 _scriptName;
		std::string		 _path;
		std::string		 _queryString;
		std::string		 _fragment;
		// Http version
		std::string		 _httpVersion;
		// Headers
		std::map <std::string, std::string>	_headers;
		// Body
		std::string		_body;
};

std::ostream &			operator<<( std::ostream & o, Request const & i );

#endif /* ********************************************************* REQUEST_H */