#ifndef MULTIPLEXER_HPP
# define MULTIPLEXER_HPP

#include "../utility/utility.hpp"

class Multiplexer
{

	public:

		Multiplexer();
		virtual ~Multiplexer();

	private:

		std::vector<struct pollfd *> _fdSet;

};

#endif
