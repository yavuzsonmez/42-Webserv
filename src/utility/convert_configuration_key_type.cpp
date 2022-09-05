#include "../../inc/configuration_key/ConfigurationKey.hpp"
#include "../../inc/utility/utility.hpp"

/**
 * @brief Converts the enum in a human readable string
 * 
 * @param keyType 
 * @return std::string 
 */
std::string convert_configuration_key_type(ConfigurationKeyType keyType)
{
	if (keyType == SERVERSTARTSEGMENT) {
		return "SERVERSTARTSEGMENT";
	}
	if (keyType == SERVER_NAME) {
		return "SERVER_NAME";
	}
	if (keyType == INVALID) {
		return "INVALID";
	}
	if (keyType == LISTEN) {
		return "LISTEN";
	}
	if (keyType == INDEX) {
		return "INDEX";
	}
	if (keyType == LOCATION) {
		return "LOCATION";
	}
	if (keyType == ROOT) {
		return "ROOT";
	}
	if (keyType == METHODS) {
		return "METHODS";
	}
	return "UNKNOWN";
}

/**
 * Printing out nested keys in the location block
 */
std::string printOutNestedKeysFromLocationBlocks(ConfigurationKey key) {
	std::cout << "LOCATION BLOCK" << std::endl;
	std::cout << "ROOT " << key.root << std::endl;
	std::cout << "LOCATION " << key.location << std::endl;
	return "";
}