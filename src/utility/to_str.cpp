#include <iostream>
#include <sstream>

template <typename T>
std::string	to_str(int input)
{
	std::stringstream	ss;
	ss << input;
	return ss.str();
}

// int	main(void)
// {
// 	int	i = 42;
// 	double d = 42;
// 	float f = 42.42;
// 	std::cout << "int: " << to_str(i) << std::endl;
// 	std::cout << "double: " << to_str(d) << std::endl;
// 	std::cout << "float: " << to_str(f) << std::endl;
// 	return (0);
// }