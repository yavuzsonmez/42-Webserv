#include "../../inc/config_file/InvalidConfigurationFile.hpp"
#include <exception>

/**
 * @brief Returns default error message
 * 
 * @return const char* 
 */
const char* InvalidConfigurationFile::what() const throw() {
	return "Configuration File is invalid. Server cannot start.";
}

/**
 * @brief Returns custom error message
 * 
 * @param message 
 * @return const char* 
 */
const char* InvalidConfigurationFile::what(char *message) const throw() {
	return message;
}