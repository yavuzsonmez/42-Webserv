#include "../../inc/configuration_key/ConfigurationKey.hpp"
#include "../../inc/debugger/DebuggerPrinter.hpp"
#include "../../inc/utility/colors.hpp"
#include "../../inc/utility/utility.hpp"


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
	this->allowedMethods = src.allowedMethods;
	this->isCurrentlyParsingLocationBlock = src.isCurrentlyParsingLocationBlock;
	this->current_line = src.current_line;
	this->raw_input = src.raw_input;
	this->cgi_path = src.cgi_path;
	this->cgi_fileending = src.cgi_fileending;
	this->redirection = src.redirection;
	this->post_max_size = src.post_max_size;
	this->nestedConfigurationKeyTypesinLocationBlock = src.nestedConfigurationKeyTypesinLocationBlock;
	this->directory_listing = src.directory_listing;
	this->not_found_error_page_path = src.not_found_error_page_path;
	this->general_error_page_path = src.general_error_page_path;
	this->not_available_page_path = src.not_available_page_path;
}

ConfigurationKey::~ConfigurationKey() {
	DebuggerPrinter debugger = debugger.getInstance();
}

ConfigurationKey & ConfigurationKey::operator = (const ConfigurationKey &src) {
	(void) src;
	return (*this);
}

/**
 * Actual constructor of configuration key class.
 * Will take raw key and value and convert it in the internal_keyvalue for better handling within the class.
 * This calls detectConfigurationType, which sets the configuratio key type..
 * @param current_line The line current_line is being used for debugging purposes.
 * @param key to key to use
 * @param value the value to use
 * @param location_block is the current key within a location block?
 * @param raw_input the original input from the configuration file
 * 
 */
ConfigurationKey::ConfigurationKey(std::string key, std::string value, bool location_block, int current_line, std::string raw_input) {
	this->isCurrentlyParsingLocationBlock = location_block;
	this->current_line = current_line;
	this->directory_listing = false;
	this->raw_input = raw_input;
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
	if (this->configurationType == INVALID) {
		throwInvalidConfigurationFileExceptionWithMessage("What do you mean with " + raw.first);
	}
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
		
		return METHODS;
	}
	if (this->isCgiExecutableKeyType(raw))
	{
		debugger.info("Detected CGI PATH key type.");
		return CGI_EXECUTABLE_PATH;
	}
	if (this->isCgiFileEndingKeyType(raw))
	{
		debugger.info("Detected CGI FILE ENDING key type.");
		return CGI_FILEENDING;
	}
	if (this->isRedirectionKeyType(raw))
	{
		debugger.info("Detected REDIRECTION key type.");
		return REDIRECTION;
	}
	if (this->isDirectoryListingConfigurationKeyType(raw))
	{
		debugger.info("Detected DIRECTORY LISTING key type.");
		return DIRECTORY_LISTING;
	}
	return INVALID;
}

/**
 * @brief Checks if key is directory listing key type
 * @param type 
 * @return true or false
 */
bool ConfigurationKey::isDirectoryListingConfigurationKeyType(internal_keyvalue raw)
{
	USE_DEBUGGER;
	if (raw.first == "directory_listing") {
		if (trim_whitespaces(raw.second) == "on") {
			this->directory_listing = true;
			return true;
		}
		if (trim_whitespaces(raw.second) == "off") {
			this->directory_listing = false;
			return true;
		}
		throwInvalidConfigurationFileExceptionWithMessage("Invalid value for directory listing key. Only 'on' or 'off' is allowed.");
	}
	this->directory_listing = false;
	return false;
}

/**
 * @brief Checks if the key is a CGI Executable Path key type. Sets the cgi path value.
 * 
 * @param raw 
 * @return true 
 * @return false 
 */
bool ConfigurationKey::isCgiExecutableKeyType(internal_keyvalue raw)
{
	USE_DEBUGGER;
	if (raw.first == KEY_EXECUTABLE_PATH && !raw.second.empty())
	{
		this->cgi_path = raw.second;
		return true;
	}
	return false;
}

/**
 * @brief Validates Redirection value
 * A redirection key type is only accepted if the redirection is a valid url or a relative path
 * 
 * @param value 
 * @return true 
 * @return false 
 */
bool ConfigurationKey::validateRedirection(std::string value)
{
	USE_DEBUGGER;
	if (validate_url(value))
		return false;
	return true;
}

/**
 * @brief Checks if the key is a redirection key type.
 * A redirection key type is only accepted if the redirection is a valid url or a relative path
 * 
 * @param raw 
 * @return true 
 * @return false 
 */
bool ConfigurationKey::isRedirectionKeyType(internal_keyvalue raw)
{
	USE_DEBUGGER;
	if (raw.first == KEY_REDIRECTION && !raw.second.empty())
	{
		if (!validate_url(raw.second))
		{
			debugger.error("Redirection value incorrect. More information can be found in the error log");
			return false;
		}
		this->redirection = raw.second;
		return true;
	}
	return false;
}

/**
 * @brief Checks if the value is a file ending, means a dot in beginning and only alpha characters.
 * No spaces in between
 * 
 * @param to_validate 
 * @return true if ending is correct
 * @return false 
 */
bool ConfigurationKey::validateCgiFileEnding(std::string to_validate)
{
	USE_DEBUGGER;
	if (to_validate.empty())
	{
		debugger.info("File ending is empty.");
		return false;
	}
	if (!isalphastring(to_validate.substr(2, to_validate.length())) || to_validate[0] != '.')
	{
		debugger.error("Invalid cgi file ending. Only alpha characters allowed and dot in beginning.");
		return false;
	}
	if (!no_whitespace_between(to_validate))
	{
		debugger.error("Invalid cgi file ending. No whitespace allowed.");
		return false;
	}
	return true;
}

/**
 * @brief Validates Post Max Size.
 * - has to have an M at the end
 * - otherwise only can contain numbers
 * 
 * @param raw 
 * @return true 
 * @return false 
 */
bool ConfigurationKey::validatePostMaxSize(std::string to_validate)
{
	USE_DEBUGGER;
	if (to_validate.empty())
	{
		debugger.error("Post max size is empty.");
		return false;
	}
	if (to_validate[to_validate.length() - 1] != 'M')
	{
		throwInvalidConfigurationFileExceptionWithMessage("Invalid post max size. Has to end with M.");
		return false;
	}
	if (!isnumberstring(to_validate.substr(0, to_validate.length() - 1)))
	{
		throwInvalidConfigurationFileExceptionWithMessage("Invalid post max size. Only numbers allowed.");
		return false;
	}
	return true;
}

/**
 * @brief Checks if the key is a post max size key type. Sets the post max size value.
 * - only numbers allowed, has to have an M at the end
 * 
 * @param raw 
 * @return true 
 * @return false 
 */
bool ConfigurationKey::isPostMaxSizeType(internal_keyvalue raw)
{
	USE_DEBUGGER;
	if (raw.first == KEY_POST_MAX_SIZE && !raw.second.empty())
	{
		if (!validatePostMaxSize(raw.second))
		{
			debugger.error("Invalid post max size.");
			return false;
		}
		std::string value = raw.second.substr(0, raw.second.length() - 1);
		this->post_max_size = stoi(value);
		return true;
	}
	return false;
}

/**
 * @brief Checks if the key is a CGI File endinng key type. Sets the cgi file ending value.
 * 
 * @param raw 
 * @return true 
 * @return false 
 */
bool ConfigurationKey::isCgiFileEndingKeyType(internal_keyvalue raw)
{
	USE_DEBUGGER;
	if (raw.first == KEY_FILEENDING && !raw.second.empty())
	{
		if (!this->validateCgiFileEnding(raw.second)) {
			throwInvalidConfigurationFileExceptionWithMessage("CGI ending need to start with a dot and cannot contain any spaces.");
		}
		this->cgi_fileending = raw.second.substr(1, raw.second.length());
		debugger.debug("TESTING CGI file ending is " + this->cgi_fileending);
		return true;
	}
	return false;
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
		debugger.info("Detected server name key type in server block.");
		return SERVER_NAME;
	}
	if (this->isListenKeyType(raw))
	{
		debugger.info("Detected listen key type in server block.");
		return LISTEN;
	}
	if (this->isRootKeyType(raw))
	{
		debugger.info("Detected ROOT key type in server block.");
		return ROOT;
	}
	if (this->isIndexKeyType(raw))
	{
		debugger.info("Detected index key type in server block.");
		return INDEX;
	}
	if (this->isLocationKeyType(raw))
	{
		debugger.info("Detected listen key type. Enabled parsing location block.");
		debugger.info("Removed: " + raw.second);
		return LOCATION;
	}
	if (this->isGeneralErrorPagePathType(raw))
	{
		debugger.info("Detected general error page path type in server block.");
		return GENERAL_ERROR_PAGE;
	}
	if (this->isNotFoundErrorPagePathType(raw))
	{
		debugger.info("Detected not found error page path type in server block.");
		return NOT_FOUND_ERROR_PAGE;
	}
	if (this->isNotAvailableErrorPagePathType(raw))
	{
		debugger.info("Detected method not  available error page path type in server block.");
		return NOT_AVAILABLE_PAGE;
	}
	if (this->isPostMaxSizeType(raw))
	{
		debugger.info("Detected post max size type in server block.");
		return POST_MAX_SIZE;
	}
	if (this->isCgiExecutableKeyType(raw))
	{
		debugger.info("Detected CGI PATH key type in server block.");
		return CGI_EXECUTABLE_PATH;
	}
	if (this->isCgiFileEndingKeyType(raw))
	{
		debugger.info("Detected CGI FILE ENDING key type in server block.");
		return CGI_FILEENDING;
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
	if (raw.first == "location" && !raw.second.empty()) {
		this->isCurrentlyParsingLocationBlock = true;
		if (raw.second[raw.second.length() - 1] != '{') {
			throwInvalidConfigurationFileExceptionWithMessage("Location block does not end with {!");
		}
		raw.second.erase(raw.second.length() - 1); // delete the last character
		raw.second = trim_whitespaces(raw.second);
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
		this->root = trim_whitespaces(raw.second);
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
		substr = trim_whitespaces(substr);
		if (!substr.empty())
			this->server_names.push_back( substr );
		else
			return false;
	}
	return true;
}

/**
 * @brief Checks if a given method is valid or not.
 * - GET
 * - POST
 * - PUT
 * - DELETE
 * @param method 
 * @return true 
 * @return false 
 */
bool ConfigurationKey::isValidMethod(std::string method) {
	if (method == "GET" || method == "POST" || method == "PUT" || method == "DELETE") {
		return true;
	}
	return false;
}

/**
 * @brief Adds the method to the allowed methods. (as enum)
 * 
 * @param string methodToAdd
 * @return true 
 * @return false 
 */
void ConfigurationKey::addMethodToMethodEnum(std::string methodToAdd)
{
	USE_DEBUGGER;
	debugger.debug("ADDING " + methodToAdd + " to enum array");
	if (methodToAdd == "GET")
		this->allowedMethods.push_back(GET);
	else if (methodToAdd == "POST")
		this->allowedMethods.push_back(POST);
	else if (methodToAdd == "PUT")
		this->allowedMethods.push_back(PUT);
	else if (methodToAdd == "DELETE")
		this->allowedMethods.push_back(DELETE);
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
		substr = trim_whitespaces(substr);
		if (!this->isValidMethod(substr)) {
			throwInvalidConfigurationFileExceptionWithMessage("Invalid method: " + substr);
		}
		if (!substr.empty())
		{
			this->methods.push_back( substr );
			this->addMethodToMethodEnum(substr);
		}
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
	if (raw.first == KEY_SERVER_START_SEGMENT && raw.second == "{")
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
		substr = trim_whitespaces(substr);
		if (!substr.empty())
		{
			if (!is_digits(raw.second))
				throwInvalidConfigurationFileExceptionWithMessage("Invalid ports!");
			std::istringstream portToCheck(substr);
			portToCheck >> val;
			if (val > 65535)
				throwInvalidConfigurationFileExceptionWithMessage("Port too high.");
			if (this->validatePort(val))
				this->ports.push_back( val );
			else
				throwInvalidConfigurationFileExceptionWithMessage("Invalid port");
		}
		else
			return false;
	}
	return true;
}

/**
 * @brief Checks if the given key is a path to the not found error page or not
 * 
 * @param raw 
 * @return true is error page path
 * @return false is not error path path
 */
bool ConfigurationKey::isNotFoundErrorPagePathType(internal_keyvalue raw) {
	if (raw.first == KEY_NOT_FOUND_PAGE && !raw.second.empty())
	{
		if (this->not_found_error_page_path.empty())
			this->not_found_error_page_path = raw.second;
		else
			throwInvalidConfigurationFileExceptionWithMessage("Multiple not found error page paths!");
		return true;
	}
	return false;
}

/**
 * @brief Checks if the given key is a path to the general error page or not
 * 
 * @param raw 
 * @return true is error page path
 * @return false is not error path path
 */
bool ConfigurationKey::isGeneralErrorPagePathType(internal_keyvalue raw) {
	if (raw.first == KEY_GENERAL_ERROR_PAGE && !raw.second.empty())
	{
		if (this->general_error_page_path.empty())
			this->general_error_page_path = raw.second;
		else
			throwInvalidConfigurationFileExceptionWithMessage("Multiple not general error page paths!");
		return true;
	}
	return false;
}

/**
 * @brief Checks if the given key is a path to the not available error page key or not
 * 
 * @param raw 
 * @return true is not available page path
 * @return false is not available path path
 */
bool ConfigurationKey::isNotAvailableErrorPagePathType(internal_keyvalue raw) {
	if (raw.first == KEY_NOT_AVAILABLE_PAGE && !raw.second.empty())
	{
		if (this->not_available_page_path.empty())
			this->not_available_page_path = raw.second;
		else
			throwInvalidConfigurationFileExceptionWithMessage("Multiple not available error page paths!");
		return true;
	}
	return false;
}

/**
 * @brief Sets location block parsing to a determined value
 * - to be enabled when the location block is detected
 */
void ConfigurationKey::setLocationBlockParsing(bool value) {
	this->isCurrentlyParsingLocationBlock = value;
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
 * @brief Checks if a string only contains digits, left-padding zeros or just zeros.
 * 
 */
bool ConfigurationKey::is_digits(const std::string &str)
{
	if (str.find_first_not_of("0") == std::string::npos)
		return false;
	if (str.find_first_not_of("0") != 0)
		return false;
	return str.find_first_not_of("0123456789") == std::string::npos;
}

/**
 * @brief Throws an invalid configuration message. Prints out all information known to this parsing error.
 * Exits the program by throwing an exception.
 * @param message to print
 */
void ConfigurationKey::throwInvalidConfigurationFileExceptionWithMessage(std::string message) {
	DebuggerPrinter debugger = debugger.getInstance();
	std::cout << R << "----------------- FAILED -----------------" << Reset << std::endl;
	std::cout << R << "Encountered a problem with configuration on line " << (current_line + 1) << Reset << ":" << std::endl;
	std::cout << (current_line) << " ..." <<  std::endl;
	std::cout << (current_line + 1) << " " << Y << raw_input << Reset << std::endl;
	std::cout << (current_line + 2) << " ..." <<  std::endl;
	debugger.error(message);
	std::cout << R << "The parser did not continue after finding this error. There may be more." << std::endl;
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