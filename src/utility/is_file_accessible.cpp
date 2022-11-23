#include "../../inc/utility/utility.hpp"

//check if file exists and permission
bool is_file_accessible(std::string const &path)
{
	return (access(path.c_str(), F_OK) != -1);
}


// int	main(int argc, char**argv)
// {
// 	(void)argc;
// 	std::string my_file = "../../test";
// 	std::cout << is_file_accessible(my_file) << std::endl;
// 	return (0);
// }