#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <vector>
# include <ctime>
# include "Cgi.hpp"
# include "../utility/utility.hpp"
# include "../configuration_key/ServerBlock.hpp"
# include "Request.hpp"
# include "Location.hpp"

/**
 * @brief class that is used to generate the response
 */

class Response
{
	public:
	Response();
	Response(Request request, ServerBlock config);
	~Response(void);

	void	process_request(void);
	void	create_response(void);
	std::string	get_response(void);
	

	void	test_text(void);
	void	test_html(void);
	void	test_image(void);
	void	test_cgi(ServerBlock &config);

	std::string	get_image_format(void);

	private:
	Request		_request;
	ServerBlock	_config;
	Location	_location;
	std::string	_protocol;
	std::string	_status_code;
	std::string	_status_text;

	std::string	_server;
	std::string	_content_type;
	std::string	_charset;
	std::string	_content_length;
	std::string	_body;
	// std::string _html;
	// std::string _plain;
	// std::string _image;

	std::string	_response;

};


# endif