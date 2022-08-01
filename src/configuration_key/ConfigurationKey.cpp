#include "../../inc/ConfigurationKey.hpp"

/**
 * Default constructor
 */
ConfigurationKey::ConfigurationKey() {
    throw{"Unsupported constructor. Use key value constructor instead!"};
}

ConfigurationKey::ConfigurationKey( const ConfigurationKey &src ) {

}

ConfigurationKey::~ConfigurationKey() {

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