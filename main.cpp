#include "inc/utility.hpp"
#include "inc/request.hpp"
#include "inc/ConfigFileParsing.hpp"

/**
 * @brief Checks the argument count of the program and the filename of the configuration
 * Returns true on success.
 */
bool check_arguments_and_filename(int argc, char**argv)
{
	if (argc != 2 || !check_config_file(argv[1]))
	{
		std::cout << "usage: ./webserv [path/webserv.conf]" << std::endl;
		return (false);
	}
	return (true);
}

/**
 * Entrypoint
 * Validates input and then starts the configuration file parsing.
 */
int main(int argc, char **argv)
{
	if (!check_arguments_and_filename(argc, argv)) return (1);

	ConfigFileParsing configurationFileParsing;
	std::string file_content;
	file_content = get_file_content(argv[1]);

	std::cout << argv[1] << std::endl;
	try {
		configurationFileParsing.parseConfigFile(file_content);
	} catch (const std::exception& e) {
		std::cout << "TEST";
		std::cout << "error: " << e.what() << std::endl;
		return (1);
	}
	return (0);
}