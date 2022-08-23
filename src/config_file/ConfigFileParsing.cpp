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
 * @brief Adds a new configuration key to the current server block.
 * If block is SERVERSTARTSEGMENT, a new server will be created in the vector.
 * All other keys will be added. If a keys is invalid, the program
 * will stop exection immediately.
 * 
 * @param key ConfigurationKey from determineConfigurationKeys
 */
void ConfigFileParsing::addConfigurationKeyToCurrentServerBlock( ConfigurationKey key )
{
	USE_DEBUGGER;
	int static currentServerIndex = 0;

	// creating a new server
	if (key.configurationType == SERVERSTARTSEGMENT) {
		debugger.debug("CREATING NEW SERVER BLOCK ");
		serverBlocks.push_back(ServerBlock());
	}
	else
	{
		debugger.debug("Adding key to server block " + std::to_string(currentServerIndex));
		if (serverBlocks.size() == 0) {
			debugger.error("No server block found or key is out of scope.");
			throw InvalidConfigurationFile();
		}
		serverBlocks[currentServerIndex].addConfigurationKey(key);
	}
}

/**
 * @brief Determines if the line in the configuration file should be skipped or not
 * - As to be skipped qualifies any line starting with a closing curly bracket: }
 * or a line with only empty spaces
 * @return true 
 * @return false 
 */
bool ConfigFileParsing::shouldSkipLineInConfigurationFile(std::string line, int firstNotWhiteSpacePosition)
{
	USE_DEBUGGER;
	if (firstNotWhiteSpacePosition == std::string::npos) {
		debugger.debug("SKIPPING: Line is empty.");
		return true;
	}
	if (line[firstNotWhiteSpacePosition] == '}') {
		debugger.debug("SKIPPING: Line is a closing curly bracket.");
		return true;
	}
	return false;
}

/**
 * @brief Parses a configuration file into configuration keys and adds them to server blocks.
 * 
 * Determines the configuration for a entry line by line.
 * Takes file_content and prints out the detected ConfigurationType.
 * It skips empty lines automatically.
 * When iterating, it trims the string from the left side.
 * Still in testing.
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
		if (shouldSkipLineInConfigurationFile(line, firstNotWhiteSpacePosition)) {
			lineNumber++;
			continue;
		}
		std::string trimmedString = line.replace(0, firstNotWhiteSpacePosition, "");
		// now splitting string up
		std::vector<std::string> key_value_raw = split_once_on_delimiter(trimmedString, ' ');
		debugger.debug("KEY TO USE \033[0;34m" + key_value_raw[0] + " \033[0m VALUE TO USE \033[0;34m" + key_value_raw[1] + "\033[0m");
		ConfigurationKey key = ConfigurationKey(key_value_raw[0], key_value_raw[1]);
		debugger.debug("LINE " + std::to_string(lineNumber) + ": " + key.key);
		addConfigurationKeyToCurrentServerBlock(key);
		lineNumber++;
	}
}