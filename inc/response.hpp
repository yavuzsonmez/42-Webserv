#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <ctime>

class response
{
	public:
	response();
	~response(void);

	private:
	std::string	protocol;
	std::string	status_code;
	std::string	status_text;

	std::string	server;
	tm	*date;
	std::string	content_type;
	std::string	charset;

};


# endif