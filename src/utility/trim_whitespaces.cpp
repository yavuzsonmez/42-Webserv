#include "../../inc/utility/utility.hpp"

/**
 * @brief Strips all whitespaces left and right, does not touch whitespaces in the string itself
 * @param std::string string to strip whitespaces from
 */
std::string trim_whitespaces(std::string str)
{
	size_t start = str.find_first_not_of(' ');
	size_t end = str.find_last_not_of(' ');
	return (str.substr(start, end - start + 1));
}

// int	main(void)
// {
// 	std::string	str = "    123456789     ";
// 	std::cout << "string: " << str << std::endl;
// 	std::cout << "len: " << str.size() << std::endl;
// 	str = trim_whitespaces(str);
// 	std::cout << "string: " << str << std::endl;
// 	std::cout << "len: " << str.size() << std::endl;
// 	return (0);
// }