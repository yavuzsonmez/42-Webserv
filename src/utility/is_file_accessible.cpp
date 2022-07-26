#include "../../inc/utility.hpp"

//check if file exists and permission
bool is_file_accessible(std::string const &path)
{
	std::ifstream my_file(path);
	if (!my_file.fail() && my_file.good())
		return true;
	return false;
}


// int	main(int argc, char**argv)
// {
// 	(void)argc;
// 	std::string my_file = "../../test";
// 	std::cout << is_file_accessible(my_file) << std::endl;
// 	return (0);
// }