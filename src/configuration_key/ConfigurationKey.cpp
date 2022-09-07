#include "../../inc/configuration_key/ConfigurationKey.hpp"
#include "../../inc/debugger/DebuggerPrinter.hpp"


/**
 * Default constructor
 */
ConfigurationKey::ConfigurationKey() {
	throw("Unsupported constructor. Use key value constructor instead!");
}

/**
 * @brief Copy constructor
 * Add any new members to this constructor.
 * 
 * @param src 
 */
ConfigurationKey::ConfigurationKey( const ConfigurationKey &src ) {
	USE_DEBUGGER;
	this->key = src.key;
	this->value = src.value;
	this->configurationType = src.configurationType;
	this->ports = src.ports;
	this->server_names = src.server_names;
	this->root = src.root;
	this->location = src.location;
	this->indexes = src.indexes;
	this->methods = src.methods;
	this->isCurrentlyParsingLocationBlock = src.isCurrentlyParsingLocationBlock;
}

ConfigurationKey::~ConfigurationKey() {
	DebuggerPrinter debugger = debugger.getInstance();
}

ConfigurationKey & ConfigurationKey::operator = (const ConfigurationKey &src) {
	return (*this);
}

/**
 * Actual constructor of configuration key class.
 * Will take raw key and value and convert it in the internal_keyvalue for better handling within the class.
 * This calls detectConfigurationType, which sets the configuratio key type.
 */
ConfigurationKey::ConfigurationKey(std::string key, std::string value, bool location_block) {
	this->isCurrentlyParsingLocationBlock = location_block;
	DebuggerPrinter debugger = debugger.getInstance();
	if (key.empty () || value.empty()) {
		throwInvalidConfigurationFileExceptionWithMessage("Key or value of configuration key was empty!");
	}
	debugger.info("Constructed configuration key.");
	internal_keyvalue raw(key, value);
	if (this->isCurrentlyParsingLocationBlock) {
		this->configurationType = detectLocationKeyConfiguration(raw);
	} else {
		this->configurationType = detectConfigurationType(raw);
	}
	this->key = raw.first;
	this->value = raw.second;
}
/**
 * @brief Detects the type of configuration key for the location block.
 * - Configures the location block keys.
 */
ConfigurationKeyType ConfigurationKey::detectLocationKeyConfiguration(internal_keyvalue &raw) {
	USE_DEBUGGER;
	debugger.info("Those keys are for location block");
	if (this->isIndexKeyType(raw))
	{
		debugger.info("Detected index key type for location.");
		return INDEX;
	}
	if (this->isRootKeyType(raw))
	{
		debugger.info("Detected ROOT key type.");
		return ROOT;
	}
	if (this->isMethodsKeyType(raw))
	{
		debugger.info("Detected METHODS key type.");
		return ROOT;
	}
	return INVALID;
}


/**
 * @param Returns the correct configuration key based on the key and value.
 *
 * If the key is invalid or not yet implemented, it returns INVALID. This should
 * be treated as fatal error.
 * Some keys and values will be cleaned, that is way we have to pass a reference to the function
 */
ConfigurationKeyType ConfigurationKey::detectConfigurationType(internal_keyvalue &raw) {
	USE_DEBUGGER;
	if (this->isServerStartSegment(raw))
	{
		debugger.info("Detected server start segment.");
		return SERVERSTARTSEGMENT;
	}
	if (this->isServerNameKeyType(raw))
	{
		debugger.info("Detected server name key type.");
		return SERVER_NAME;
	}
	if (this->isListenKeyType(raw))
	{
		debugger.info("Detected listen key type.");
		return LISTEN;
	}
	if (this->isRootKeyType(raw))
	{
		debugger.info("Detected ROOT key type.");
		return ROOT;
	}
	if (this->isIndexKeyType(raw))
	{
		debugger.info("Detected index key type.");
		return INDEX;
	}
	if (this->isLocationKeyType(raw))
	{
		debugger.info("Detected listen key type. Enabled parsing location block.");
		debugger.info("Removed: " + raw.second);
		return LOCATION;
	}
	return INVALID;
}

/**
 * @brief Returns true if the location is a location key type.
 * - isCurrentlyParsingLocationBlock
 * - Will check if the last character in location is a opening bracket
 * - Will remove the last character from location if it is a opening bracket to enable parsing of the location path
 */
bool ConfigurationKey::isLocationKeyType(internal_keyvalue &raw) {
	if (raw.first == "location") {
		this->isCurrentlyParsingLocationBlock = true;
		if (raw.second[raw.second.length() - 1] != '{') {
			throwInvalidConfigurationFileExceptionWithMessage("Location block does not end with {!");
		}
		raw.second.pop_back();
		return true;
	}
	return false;
}

/**
 * @brief Checks if the key is a root key type. Sets the root value.
 * 
 * @param raw 
 * @return true 
 * @return false 
 */
bool ConfigurationKey::isRootKeyType(internal_keyvalue raw)
{
	if (raw.first == KEY_ROOT && !raw.second.empty())
	{
		this->root = raw.second;
		return true;
	}
	return false;
}


/**
 * If the internal_keyvalue is of type INDEX this will return true and set the according values
 * in the class.
 * 
 * Then it adds indexes, seperated by spaces.
 */
bool ConfigurationKey::isIndexKeyType(internal_keyvalue raw)
{
	if (raw.first != KEY_INDEX)
		return false;
	
	std::stringstream ss(raw.second);
	while (ss.good())
	{
		std::string substr;
		std::getline( ss, substr, ' ' );
		if (!substr.empty())
			this->indexes.push_back( substr );
		else
			return false;
	}
	return true;
}

/**
 * If the internal_keyvalue is of type SERVER_NAME this will return true and set the according values
 * in the class.
 * If not, it will not set anything and just return false.
 * - It checks if server_name is correct.
 * Then it adds server names, seperated by spaces.
 */
bool ConfigurationKey::isServerNameKeyType(internal_keyvalue raw) {
	if (raw.first != KEY_SERVER_NAMES)
		return false;
	
	std::stringstream ss(raw.second);
	while (ss.good())
	{
		std::string substr;
		std::getline( ss, substr, ' ' );
		if (!substr.empty())
			this->server_names.push_back( substr );
		else
			return false;
	}
	return true;
}

/**
 * @brief Check if the key is a methods key type.
 * - also sets the methods
 * TODO: Validate methods values
 */
bool ConfigurationKey::isMethodsKeyType(internal_keyvalue raw) {
	if (raw.first != KEY_METHODS)
		return false;
	
	std::stringstream ss(raw.second);
	while (ss.good())
	{
		std::string substr;
		std::getline( ss, substr, ' ' );
		std::cout << "METHODS ADDING: " << substr << std::endl;
		if (!substr.empty())
			this->methods.push_back( substr );
		else
			return false;
	}
	return true;
}

/**
 * @brief Is serverblock start segment
 * Returns true if the next segment is a server block start segment.
 * It looks like this: server {
 * 
 * @param raw 
 * @return true or false
 */
bool ConfigurationKey::isServerStartSegment(internal_keyvalue raw) {
	if (raw.first == "server" && raw.second == "{")
		return true;
	return false;
}


/**
 * If the internal_keyvalue is of type LISTE this will return true and set the according values
 * in the class.
 * If not, it will not set anything and just return false.
 * - It checks if port is correct.
 * Then it adds server ports, seperated by spaces.
 */
bool ConfigurationKey::isListenKeyType(internal_keyvalue raw) {
	if (raw.first != KEY_LISTEN)
		return false;
	
	std::stringstream ss(raw.second);
	while (ss.good())
	{
		unsigned int val;
		std::string substr;
		std::getline( ss, substr, ' ' );
		if (!substr.empty())
		{
			if (!is_digits(raw.second))
				throwInvalidConfigurationFileExceptionWithMessage("Invalid ports!");
			std::istringstream portToCheck(substr);
			portToCheck >> val;
			if (this->validatePort(val))
				this->server_names.push_back( substr );
			else
				throw("Unsupported constructor. Use key value constructor instead!");
		}
		else
			return false;
	}
	return true;
}

/**
 * @brief Sets location block parsing to a determined value
 * - to be enabled when the location block is detected
 */
void ConfigurationKey::setLocationBlockParsing(bool value) {
	this->isCurrentlyParsingLocationBlock = false;
}

/**
 * @brief Checks if the given port is a valid port.
 * Currently checking:
 * - Negative number
 * - Max Port
 *
 * @returns bool - true or false
 * @TODO: Check if anything else has to be checked
 */
bool ConfigurationKey::validatePort(unsigned int port) {
	if (port > 65535)
		return false;
	if (port <= 0)
		return false;
	return true;
}

/**
 * @brief Checks if a string only contains digits.
 * 
 */
bool ConfigurationKey::is_digits(const std::string &str)
{
	return str.find_first_not_of("0123456789") == std::string::npos;
}

/**
 * Throws an InvalidConfigurationFile error with a message in front
 * printed by the debugger.
 *
 * @param message to print
 */
void ConfigurationKey::throwInvalidConfigurationFileExceptionWithMessage(std::string message) {
	DebuggerPrinter debugger = debugger.getInstance();
	debugger.error(message);
	throw InvalidConfigurationFile();
}

//// Testing main and usage examples
//int main() {
//	// test for server name
//	ConfigurationKey key("server_names", "localhost localhost.localdomain");

//	// test for listen
//	ConfigurationKey key2("listen", "8080 808080");

//	// test for invalid key
//	try {
//		ConfigurationKey key3("invalid", "value");
//	} catch (std::runtime_error error) {
//		std::cout << "Error for invalid key as expected!" << std::endl;
//	}

//	// test for server start segment
//	ConfigurationKey key4("server", "{");


//	return 0;
//}