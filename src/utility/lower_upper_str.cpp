#include "../../inc/utility/utility.hpp"

/**
 * @brief Is number string
 * 
 * @param str 
 * @return true 
 * @return false 
 */
bool isnumberstring(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (!isdigit(str[i]))
			return false;
	}
	return true;
}

/**
 * @brief Tests if a string only contains alpha characters
 * 
 * @param str 
 */
bool isalphastring(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (!isalpha(str[i]))
			return false;
	}
	return true;
}

/**
 * @brief Checks if string does not consist any whitespaces between characters
 * 
 * @param str 
 * @return true if there are no spaces in the string
 */
bool no_whitespace_between(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (isspace(str[i]))
			return false;
	}
	return true;
}


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