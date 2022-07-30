#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <vector>
# include <ctime>
# include "utility.hpp"

class response
{
	public:
	response();
	~response(void);

	std::string	get_response(void);
	void	create_response(void);

	void	test_text(void);
	void	test_html(void);
	void	test_image(void);
	void	read_html(std::string path);

	private:
	std::string	_protocol;
	std::string	_status_code;
	std::string	_status_text;

	std::string	_server;
	tm	*_date;
	std::string	_content_type;
	std::string	_charset;
	std::string	_content_length;
	std::string _html;
	std::string _plain;

	std::string	_response;

};


# endif