#include "../../inc/config_file/InvalidConfigurationFile.hpp"
#include <exception>

const char* InvalidConfigurationFile::what() const throw() {
	return "Configuration File is invalid. Server cannot start.";
}