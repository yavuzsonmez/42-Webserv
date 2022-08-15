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
 * This calls detectConfigurationType.
 */
ConfigurationKey::ConfigurationKey(std::string key, std::string value) {
    DebuggerPrinter debugger = debugger.getInstance();
    debugger.info("Constructed configuration key.");
    internal_keyvalue raw(key, value);
    ConfigurationKeyType configurationType = detectConfigurationType(raw);
}

/**
 * @param Returns the correct configuration key based on the key and value.
 * 
 * If the key is invalid or not yet implemented, it returns INVALID. This should
 * be treated as fatal error.
 */
ConfigurationKeyType ConfigurationKey::detectConfigurationType(internal_keyvalue raw) {
    return INVALID;
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