
#include "../../inc/utility.hpp"


//Remove in file_content everything between start (included) to end (excluded)
//Remove comment (starting with #) from file content
void strip_from_str(std::string &file_content, const char start, const char end)
{
	size_t start_index = file_content.find(start);
	size_t end_index = file_content.find(end);

	std::string::iterator it_s = file_content.begin() + start_index;
	std::string::iterator it_e = file_content.begin() + end_index;

	while ( start_index != std::string::npos)
	{
		file_content.replace(it_s, it_e, "\n");
		start_index = file_content.find(start);
		end_index = file_content.find(end);
		it_s = file_content.begin() + start_index;
		it_e = file_content.begin() + end_index;
	}

}


// int	main(int argc, char**argv)
// {

// 	std::string config_file = "hello world; #q powngfpoqwngpo\nlisten	8000;\n#hihihih;";

// 	std::cout << config_file << std::endl;
// 	std::cout << "____________________" << std::endl;
// 	strip_from_str(config_file, '#', '\n');
// 	std::cout << config_file << std::endl;
// 	return (0);
// }