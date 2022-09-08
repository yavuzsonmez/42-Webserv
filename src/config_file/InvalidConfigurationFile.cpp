#include "../../inc/config_file/InvalidConfigurationFile.hpp"
#include "../../inc/debugger/DebuggerPrinter.hpp"
#include "../../inc/utility/colors.hpp"
#include <exception>

/**
 * @brief Returns default error message
 * 
 * @return const char* 
 */
const char* InvalidConfigurationFile::what() const throw() {
	return "Configuration File is invalid. Server cannot start.";
}

const char* InvalidConfigurationFile::what(char *message, int current_line, char *raw_input)  const throw() {
	USE_DEBUGGER;
	std::cout << R << "----------------- FAILED -----------------" << Reset << std::endl;
	std::cout << R << "Encountered a problem with configuration on line " << (current_line + 1) << Reset << ":" << std::endl;
	std::cout << (current_line) << " ..." <<  std::endl;
	std::cout << (current_line + 1) << " " << Y << raw_input << Reset << std::endl;
	std::cout << (current_line + 2) << " ..." <<  std::endl;
	debugger.error(message);
	std::cout << R << "The parser did not continue after finding this error. There may be more." << std::endl;
	return message;
}