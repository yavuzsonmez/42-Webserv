#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>
# include <map>

enum method { GET, POST, DELETE };
enum protocol { http, https };

class Request
{

	public:

		Request();
		Request( Request const & src );
		~Request();

		Request &		operator=( Request const & rhs );

	private:

		/* METHOD */
		unsigned short _method; //method

		/* URL */
		std::string _protocol;
		std::string _domain;
		unsigned int _port;
		std::string _scriptName;
		std::string _path;
		std::string _queryString;
		std::string _fragment;

		/* HTTP VERSION */
		std::string _httpVersion;

		/* HEADERS */
		std::map <std::string, std::string> _headers;

		/* BODY */
		std::string _body;
};

std::ostream &			operator<<( std::ostream & o, Request const & i );

#endif /* ********************************************************* REQUEST_H */