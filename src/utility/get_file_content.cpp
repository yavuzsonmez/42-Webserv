
#include "../../inc/utility/utility.hpp"

std::string get_file_content(std::string path)
{
	std::ifstream f(path); //taking file as inputstream
	if (!f)
		throw (500);
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