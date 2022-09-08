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