#include "../../inc/config_file/ConfigFileParsing.hpp"
#include "../../inc/utility/utility.hpp"
#include "../../inc/configuration_key/ConfigurationKey.hpp"
#include "../../inc/debugger/DebuggerPrinter.hpp"
#include "../../inc/utility/colors.hpp"
#include "../../inc/http/Request.hpp"
#include <set>

ConfigFileParsing::ConfigFileParsing()
{
	this->isCurrentlyInLocationBlock = false;
}

ConfigFileParsing::ConfigFileParsing( const ConfigFileParsing &src )
{
	this->isCurrentlyInLocationBlock = src.isCurrentlyInLocationBlock;
	this->serverBlocks = src.serverBlocks;
}

ConfigFileParsing::~ConfigFileParsing()
{

}

ConfigFileParsing & ConfigFileParsing::operator = (const ConfigFileParsing &src) {
	(void) src;
	return (*this);
}

/**
 * @brief Checks for duplicates in the config file which are a logic mistake
 * * - checking double ports on all server blocks
 * - checking double server names on all server blocks
 * - checking double location paths for each server block
 * - checking double ports on all server blocks
 * - checking double post max body size for each server block
 * @return true 
 * @return false 
 */
bool ConfigFileParsing::validationDuplicationCheck() {
	USE_DEBUGGER;
	std::vector<unsigned int> allServerPorts = getAllServerPortsFromAllServerBlocks(this->serverBlocks);
	std::vector<std::string> allServerNames = getAllServerNamesFromAllServerBlocks(this->serverBlocks); 
	if (checksIfAnyServerBlockHasDoubleErrorPages(this->serverBlocks)) {
		debugger.error("Error: Double error pages found in configuration file.");
		throw InvalidConfigurationFile();
	}
	if (vector_has_duplicate_element(allServerPorts)) {
		debugger.error("Configuration file has duplicate ports.");
		throw InvalidConfigurationFile();
	}
	if (vector_has_duplicate_element(allServerNames)) {
		debugger.error("Configuration file has duplicate server names.");
		throw InvalidConfigurationFile();
	}
	if (!checkIfKeyIsUniqueInEachServerBlock(serverBlocks, POST_MAX_SIZE)) {
		debugger.error("Configuration file has duplicate post_max_body_size.");
		throw InvalidConfigurationFile();
	}
	return true;
}

/**
 * GENERAL VALIDATION FUNCITON
 * 
 * @brief To be run after parsing. Checks if the configuration file is valid. (e.g. no double ports, no double server names)
 * - runs validationDuplicationCheck(), checking for logic duplicates
 * - checks that there is a root available in each server block
 * - checks that for every location cgi there is a file ending available
 * @return true 
 * @return false 
 */
bool ConfigFileParsing::validateConfiguration() {
	USE_DEBUGGER;

	if (validationDuplicationCheck()) {
		debugger.debug("Configuration file has no detected duplicates in ports, server names, error pages or post_max_body_size.");
	}
	if (!keyExistsInEachServerBlock(serverBlocks, ROOT)) {
		debugger.error("Configuration file has no root defined in one or more server blocks.");
		throw InvalidConfigurationFile();
	}
	if (!checkIfCgiExecutableAndFileEndingAreSet(serverBlocks)) {
		debugger.error("Configuration file has no cgi_fileending defined in one or more location blocks.");
		throw InvalidConfigurationFile();
	}
	if (!keyExistsInEachLocationBlock(serverBlocks, ROOT)) {
		debugger.error("At least one location block does not contain a ROOT option. This is required.");
		throw InvalidConfigurationFile();
	}

	return true;
}

/**
 * @brief Parses the given content from the content file
 * @note Removes all the comments from a configuration file. (a comment starts with #)
 * @param file_content
 * @return true / false
 * @note Runs two checks. First check is to see if the file is faulty. If it is, it will return false.
 * Next check is actual logic of the file, means no double server ports etc...
 */
bool ConfigFileParsing::parseConfigFile( std::string &file_content ) {
	strip_from_str(file_content, '#', '\n');
	if (isGeneralFaultyFile(file_content)) {
		return false;
	}
	determineConfigurationKeys(file_content);
	return validateConfiguration();
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
 * @brief Add the a configuration key type to the location block type collection
 * Is being used to document which keys were registerd and which not.
 * This is maninly done for validation purposes.
 * 
 * @param key_type 
 */
void ConfigFileParsing::addConfigurationKeyTypeToLocation(ConfigurationKeyType keyType, ConfigurationKey &keyToAdd) {
	keyToAdd.nestedConfigurationKeyTypesinLocationBlock.push_back(keyType);
}

/**
 * @brief Add a key to a location block key (which is the last key in the vector)
 * @param key original location key
 * @param keyToAdd key to add to the original location key
 * ALL KEYS WHICH ARE HELD BY LOCATION BLOCKS HAVE TO BE ADDED IN THIS FUNCTION
 * Super important function, without adding behavior for key it it will not appear in the location block!
 */
void ConfigFileParsing::addConfigurationKeyToLocation( ConfigurationKey &key, ConfigurationKey keyToAdd ) {
	if (keyToAdd.configurationType == INDEX) {
		key.indexes = keyToAdd.indexes;
		addConfigurationKeyTypeToLocation(INDEX, key);
	}
	else if (keyToAdd.configurationType == LOCATION) {
		key.location = trim_whitespaces(keyToAdd.location);
		addConfigurationKeyTypeToLocation(LOCATION, key);
	}
	else if (keyToAdd.configurationType == ROOT) {
		key.root = trim_whitespaces(keyToAdd.root);
		addConfigurationKeyTypeToLocation(ROOT, key);
	}
	else if (keyToAdd.configurationType == METHODS) {
		key.methods = keyToAdd.methods;
		addConfigurationKeyTypeToLocation(METHODS, key);
	}
	else if (keyToAdd.configurationType == CGI_EXECUTABLE_PATH) {
		key.cgi_path = trim_whitespaces(keyToAdd.cgi_path);
		addConfigurationKeyTypeToLocation(CGI_EXECUTABLE_PATH, key);
	}
	else if (keyToAdd.configurationType == CGI_FILEENDING) {
		key.cgi_fileending = trim_whitespaces(keyToAdd.cgi_fileending);
		addConfigurationKeyTypeToLocation(CGI_FILEENDING, key);
	}
	else if (keyToAdd.configurationType == REDIRECTION) {
		key.redirection = trim_whitespaces(keyToAdd.redirection);
		addConfigurationKeyTypeToLocation(REDIRECTION, key);
	} else {

	}
}

/**
 * @brief Adds a new configuration key to the current server block.
 * If block is SERVERSTARTSEGMENT, a new server will be created in the vector.
 * All other keys will be added. If a keys is invalid, the program
 * will stop exection immediately.
 * - adds either to the server block or the last location key.
 * 
 * @param key ConfigurationKey from determineConfigurationKeys
 */
void ConfigFileParsing::addConfigurationKeyToCurrentServerBlock( ConfigurationKey &key )
{
	USE_DEBUGGER;
	int static currentServerIndex = -1;
	
	if (this->isCurrentlyInLocationBlock && this->isCurrentlyInServerBlock) {
		this->addConfigurationKeyToLocation(this->serverBlocks[currentServerIndex].configurationKeys.back(), key);
		return;
	}
	// set location block to true if location was detected
	if (key.configurationType == LOCATION && this->isCurrentlyInServerBlock) {
		this->isCurrentlyInLocationBlock = true;
		this->server_bracket_counter++;
	}

	// creating a new server
	// set SERVER BLOCK to true
	if (key.configurationType == SERVERSTARTSEGMENT) {
		currentServerIndex++;
		this->server_bracket_counter++;
		serverBlocks.push_back(ServerBlock());
		this->isCurrentlyInServerBlock = true;
	}
	else
	{
		if (this->server_bracket_counter == 0) {
			debugger.error("Configuration file has no server block.");
			throw InvalidConfigurationFile();
		}
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
 * - disables configuration key parsing if necessary
 * @return true 
 * @return false 
 */
bool ConfigFileParsing::shouldSkipLineInConfigurationFile(std::string line, int firstNotWhiteSpacePosition)
{
	USE_DEBUGGER;
	if (firstNotWhiteSpacePosition ==  (int) std::string::npos) {
		debugger.debug("SKIPPING: Line is empty.");
		return true;
	}
	if (line[firstNotWhiteSpacePosition] == '}' || line[firstNotWhiteSpacePosition] == '{') {
		if (line[firstNotWhiteSpacePosition] == '}') {
			this->server_bracket_counter--;
		} else {
			this->server_bracket_counter++;
		}
	}
	if (line[firstNotWhiteSpacePosition] == '}') {
		debugger.debug("SKIPPING: Line is a closing curly bracket.");
		this->isCurrentlyInLocationBlock = false;
		return true;
	}
	return false;
}

/**
 * @brief Parses a configuration file into configuration keys and adds them to server blocks.
 *   MAIN PARSING FUNCTION
 *   @param file_content
 * - Determines the configuration for a entry line by line.
 * - Takes file_content and prints out the detected ConfigurationType.
 * - It skips empty lines automatically.
 * - When iterating, it trims the string from the left side.
 * - Sets the location block parsing flag to true when a location block is detected. The Location block parsing
 *   is controlled by ConfigFileParsing.
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
		if (key_value_raw.size() != 2) {
			debugger.error("Invalid configuration key detected. Line: " + std::to_string(lineNumber));
			debugger.error("There is no second pair for the key value pair.");
			throw InvalidConfigurationFile();
		}
		debugger.debug("KEY TO USE \033[0;34m" + key_value_raw[0] + " \033[0m VALUE TO USE \033[0;34m" + key_value_raw[1] + "\033[0m");
		ConfigurationKey key = ConfigurationKey(key_value_raw[0], trim_whitespaces(key_value_raw[1]), this->isCurrentlyInLocationBlock, lineNumber, trimmedString);
		debugger.debug("Adding key to current server block with configuration key " + std::to_string(key.configurationType));
		debugger.debug("LINE " + std::to_string(lineNumber) + ": " + key.key);
		addConfigurationKeyToCurrentServerBlock(key);
		lineNumber++;
	}
	printAllServerBlocks(this->serverBlocks);
}

/**
 * @brief DEBUG FUNCTION prints out all configuration keys of all available server blocks
 * MAIN DEBUG PRINTING FUNCTION
 * 
 * @param serverBlocks 
 */
void ConfigFileParsing::printAllServerBlocks(std::vector<ServerBlock> &serverBlocks)
{
	if (DEBUGMODE != 1) return;
	int locationBlockCounter = 0;
	std::string intendation = "   ";
	for (int i = 0; i < (int) serverBlocks.size(); i++) {
		std::cout << R << "SERVER BLOCK " << Reset << i << std::endl;
		// print every configuration key
		for (int j = 0; j < (int) serverBlocks[i].configurationKeys.size(); j++) {
			std:: cout << intendation;
			printKeyValueColored(serverBlocks[i].configurationKeys[j].key, serverBlocks[i].configurationKeys[j].value);
			if (serverBlocks[i].configurationKeys[j].configurationType == LOCATION) {
				std:: cout << intendation;
				printOutNestedKeysFromLocationBlocks(serverBlocks[i].configurationKeys[j]);
				locationBlockCounter++;
			}
		}
	}
}