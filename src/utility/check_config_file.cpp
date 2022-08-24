#include "../../inc/utility/utility.hpp"

/**
 * @brief Checks the filename of the config file and if it is accessible
 *
 * @param path
 * @return true if successful
 * @return false
 */
bool check_config_file(std::string const &path)
{
	std::string file_name;

	if (!is_file_accessible(path))
		return (false);
	file_name = get_file_name(path);
	lower_str(file_name);
	if (file_name.compare("webserv.conf"))
		return (false);
	return (true);
}

// int	main(int argc, char**argv)
// {
// 	if (argc != 2)
// 		return 1;
// 	std::string my_file = &argv[1][0];
// 	if (check_config_file(my_file))
// 		std::cout << "The config file is fine." << std::endl;
// 	else
// 		std::cout << "Something wrong with config file." << std::endl;
// 	return (0);
// }