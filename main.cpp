
#include "inc/config_file/ConfigFileParsing.hpp"
#include "inc/configuration_key/ConfigurationKey.hpp"
#include "inc/configuration_key/ServerBlock.hpp"
#include "inc/debugger/DebuggerPrinter.hpp"
#include "inc/http/Response.hpp"
#include "inc/network/ServerSocket.hpp"
#include "inc/utility/utility.hpp"


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
	DebuggerPrinter debugger = debugger.getInstance();
 	if (!check_arguments_and_filename(argc, argv)) return (1);

	ConfigFileParsing static *configurationFileParsing = new ConfigFileParsing();
	std::string file_content;
	file_content = get_file_content(argv[1]);

	std::cout << argv[1] << std::endl;
	try {
		configurationFileParsing->parseConfigFile(file_content);
		debugger.info("CONFIG FILE OK");
	} catch (const std::exception& e) {
		std::cout << "error: " << e.what() << std::endl;
		return (1);
	}

	delete configurationFileParsing;


	/* START TEST REQUEST */
/* 	std::string get = get_file_content("./sample/http_request_get");
	std::string post = get_file_content("./sample/http_request_post");
	std::string del = get_file_content("./sample/http_request_delete");
	std::string wrong = get_file_content("./sample/http_request_wrong");
	//Request httpRequestPost(post);
	Request httpRequestGet(get);
	//Request httpRequestDelete(del);
	//Request httpRequestDelete(wrong);
	std::cout << httpRequestGet << std::endl; */
	/* END */


	/* START TEST RESPONSE */
	ServerSocket server(4242, INADDR_ANY);
	/* END */

	return (0);
}