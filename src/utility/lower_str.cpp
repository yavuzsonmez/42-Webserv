
#include "../../inc/utility.hpp"


//lowercase a string, then we can also accept more inputs
void lower_str(char *str, size_t length)
{
	for (size_t i = 0; i < length; i++)
		str[i] = tolower(str[i]);
}

int	main(int argc, char**argv)
{
	std::string str = "webSerV.conF"; //input with uppercase characters
	char *cstr = &(str[0]); //conversion to c style string
	lower_str(cstr, str.length()); //call the lowercase_str function

	std::string str2(cstr); //convert c style string to std::string
	std::cout << str2 << std::endl;
	return (0);
}