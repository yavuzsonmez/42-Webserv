#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../utility/utility.hpp"
# include "../utility/colors.hpp"
# include "status.hpp"

class Response
{

	public:

		Response();
		~Response();

		std::string getResponse() const;

	private:

		std::string	_response;
};


#endif
