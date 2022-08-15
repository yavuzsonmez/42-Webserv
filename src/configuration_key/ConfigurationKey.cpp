#include "../../inc/ConfigurationKey.hpp"
#include "../../inc/DebuggerPrinter.hpp"
#include <vector>
#include <string>
#include <sstream>

/**
 * Default constructor
 */
ConfigurationKey::ConfigurationKey() {
    throw("Unsupported constructor. Use key value constructor instead!");
}

ConfigurationKey::ConfigurationKey( const ConfigurationKey &src ) {

}

ConfigurationKey::~ConfigurationKey() {
    DebuggerPrinter debugger = debugger.getInstance();
    debugger.info("Deconstructed configuration key.");
}

ConfigurationKey & ConfigurationKey::operator = (const ConfigurationKey &src) {
	return (*this);
}

/**
 * Actual constructor of configuration key class.
 * Will take raw key and value and convert it in the internal_keyvalue for better handling within the class.
 * This calls detectConfigurationType, which sets the configuratio key type.
 */
ConfigurationKey::ConfigurationKey(std::string key, std::string value) {
    DebuggerPrinter debugger = debugger.getInstance();
    debugger.info("Constructed configuration key.");
    internal_keyvalue raw(key, value);
    this->configurationType = detectConfigurationType(raw);
}

/**
 * @param Returns the correct configuration key based on the key and value.
 * 
 * If the key is invalid or not yet implemented, it returns INVALID. This should
 * be treated as fatal error.
 */
ConfigurationKeyType ConfigurationKey::detectConfigurationType(internal_keyvalue raw) {
    if (this->isServerNameKeyType(raw))
        return SERVER_NAME;
    if (this->isListenKeyType(raw))
        return LISTEN;
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