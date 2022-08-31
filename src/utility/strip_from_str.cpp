#include "../../inc/utility/utility.hpp"

/**
 * Iterates through every character in the string.
 * When it finds the matching comment character (at beginning or within the string)
 * it replaces everything with the newline character.
 *
 * Recommended usage:
 * strip_from_str(config_file, '#', '\n');
 * - Will remove all comments from a file, no matter if they are after a keyword/parameter or not.
 */
void strip_from_str(std::string &file_content, const char start, const char end)
{
	size_t start_index;
	size_t end_index;
	std::istringstream iss(file_content);
	std::string result;

	for (std::string line; std::getline(iss, line); )
	{
		if (line.find(start) == -1)
		{
			result += line + end;
			continue;
		}
		if (line.find(start) == 0)
			continue;
		start_index = line.find(start);
		line.replace(start_index, line.length(), "\n");
		result += line;
	}
	file_content = result;
}


// int	main(int argc, char**argv)
// {

// 	std::string config_file = "TEST MESSAGE\n"
// 	"TEST MESSAGE2 # TEST\n"
// 	"#TEST MESSAGE3 # TEST\n"
// 	"TEST MESSAGE4";
// 	std::cout << config_file << std::endl;
// 	std::cout << "DONE ____________________ DONE" << std::endl;
// 	strip_from_str(config_file, '#', '\n');
// 	std::cout << config_file << std::endl;
// 	return (0);
// }