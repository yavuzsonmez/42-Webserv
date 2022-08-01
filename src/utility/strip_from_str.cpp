#include <string>
#include <iostream>
#include <sstream>


//Remove in file_content everything between start (included) to end (excluded)
//Remove comment (starting with #) from file content
void strip_from_str(std::string &file_content, const char start, const char end)
{
	size_t start_index;
	size_t end_index;
	std::istringstream iss(file_content);
	std::string result;

	for (std::string line; std::getline(iss, line); )
	{
		if (line.find("#") == -1)
			result += line + "\n";
			continue;
		start_index = line.find('#');
		std::cout << start_index << " to " << line.length() << std::endl;
		std::cout << line << std::endl;
		line.replace(start_index, line.length(), "");
		std::cout << line << std::endl;
		result += line + "\n";
	}
	file_content = result;
}


int	main(int argc, char**argv)
{

	std::string config_file = "TEST MESSAGE\nTEST MESSAGE2\n#TEST MESSAGE3\nTEST MESSAGE4";
	std::cout << config_file << std::endl;
	std::cout << "____________________" << std::endl;
	strip_from_str(config_file, '#', '\n');
	std::cout << config_file << std::endl;
	return (0);
}