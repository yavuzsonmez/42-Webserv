
#include "../../inc/utility.hpp"

//lowercase a string, then we can also accept more inputs
void lower_str(std::string &str)
{
	std::transform(str.begin(), str.end(),str.begin(), tolower);
}


//uppercase a string
void upper_str(std::string &str)
{
	std::transform(str.begin(), str.end(),str.begin(), toupper);
}

// int	main(int argc, char**argv)
// {
// 	std::string str = "webSerV.conF"; //input with uppercase characters
// 	lower_str(str); //call the lowercase_str function
// 	std::cout << str << std::endl;
// 	return (0);
// }