#include "../../inc/request.hpp"

Request::Request()
{
}

Request::Request( const Request & src )
{
}

Request::~Request()
{
}

Request &				Request::operator=( Request const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Request const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}