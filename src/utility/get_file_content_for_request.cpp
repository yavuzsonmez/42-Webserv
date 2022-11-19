
#include "../../inc/utility/utility.hpp"

/**
 * @brief Get the file content for a path. 
 * 
 * @param path 
 * @return std::string 
 * @throw 404 if file could not be opened
 */
std::string get_file_content_for_request(std::string path)
{
	std::ifstream f(path); //taking file as inputstream
	if (!f)
		throw (404);
	std::string str;

	if(f) {
		std::ostringstream ss;
		ss << f.rdbuf(); // reading data
		str = ss.str();
	}
	return (str);
}

// int	main(int argc, char**argv)
// {
// 	(void)argc;
// 	std::cout << get_file_content(argv[1]) << std::endl;
// 	return (0);
// }