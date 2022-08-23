#include "../../inc/ConfigFileParsing.hpp"
#include "../../inc/utility.hpp"
#include "../../inc/ConfigurationKey.hpp"
#include "../../inc/DebuggerPrinter.hpp"
#include "../../inc/colors.hpp"

ConfigFileParsing::ConfigFileParsing()
{
	
}

ConfigFileParsing::ConfigFileParsing( const ConfigFileParsing &src )
{

}

ConfigFileParsing::~ConfigFileParsing()
{

}

ConfigFileParsing & ConfigFileParsing::operator = (const ConfigFileParsing &src) {
	return (*this);
}

/**
 * @brief Parses the given content from the content file
 * @note Removes all the comments from a configuration file. (a comment starts with #)
 * @param file_content 
 * @return true 
 * @return false or an throw exception if the configuration file is faulty.
 */
bool ConfigFileParsing::parseConfigFile( std::string &file_content ) {
	strip_from_str(file_content, '#', '\n');
	if (isGeneralFaultyFile(file_content)) {
		return false;
	}
	determineConfigurationKeys(file_content);
	return true;
}

/**
 * @brief Checks if the file is empty, does not have valid parantheses/brackets, contains invalid symbols or
 * does not have any parantheses (body) in it.
 * 
 * @param file_content 
 * @return true if file is faulty
 */
bool ConfigFileParsing::isGeneralFaultyFile( std::string &file_content ) {
	if (file_content.empty() == true) {
		throw InvalidConfigurationFile();
	}
	if (validate_parantheses(file_content) == false) {
		throw InvalidConfigurationFile();
	}
	// add parantheses check. currently paranthesis check is causing a crash.
	return false;
}


/**
 * Determines the configuration for a entry line by line.
 * Takes file_content and prints out the detected ConfigurationType.
 * It skips empty lines automatically.
 * When iterating, it trims the string from the left side.
 * Still in testing.
 * @TODO: This function should take into account that there can be multiple servers. Currently it is not doing that!
 */
void ConfigFileParsing::determineConfigurationKeys( std::string &file_content ) {
	USE_DEBUGGER;
	std::istringstream iss(file_content);
	std::string result;
	int lineNumber = 0;

	debugger.debug("Starting to parse configuration file");
	for (std::string line; std::getline(iss, line); )
	{
		debugger.debug("Parsing line number " + std::to_string(lineNumber));
		size_t firstNotWhiteSpacePosition = line.find_first_not_of("\n\r\t");
		if (firstNotWhiteSpacePosition == std::string::npos) {
			debugger.debug("Line is empty");
			lineNumber++;
			continue;
		}
		std::string trimmedString = line.replace(0, firstNotWhiteSpacePosition, "");
		// now splitting string up
		std::vector<std::string> key_value_raw = split_once_on_delimiter(trimmedString, ' ');
		debugger.debug("KEY TO USE \033[0;34m" + key_value_raw[0] + " \033[0m VALUE TO USE \033[0;34m" + key_value_raw[1] + "\033[0m");
		lineNumber++;
	}
}