#include "../../inc/utility.hpp"

//return the file name, at the end of the path
std::string get_file_name(std::string const &path)
{
	size_t pos = path.find_last_of('/');

	if (pos == std::string::npos)
		return (path);
	return (path.substr(pos + 1));
}


// int	main(int argc, char**argv)
// {
// 	if (argc != 2)
// 		return 1;
// 	std::string path = &argv[1][0];
// 	std::string file_name = get_file_name(path);
// 	std::cout << file_name << std::endl;
// 	return (0);
// }