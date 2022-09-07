#include "../../inc/configuration_key/ConfigurationKey.hpp"
#include "../../inc/utility/utility.hpp"
#include "../../inc/utility/colors.hpp"

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
 * @brief Converts the vector to a joined string with delimiter delimiter
 */
std::string join_vector(std::vector<std::string> &vec, std::string delimiter)
{
	std::string joined = "";
	for (size_t i = 0; i < vec.size(); i++) {
		joined += vec[i];
		if (i != vec.size() - 1) {
			joined += delimiter;
		}
	}
	return joined;
}

/**
 * Printing out nested keys in the location block
 */
std::string printOutNestedKeysFromLocationBlocks(ConfigurationKey &key) {
	std::cout << B << "ROOT " << key.root << Reset << std::endl;
	std::cout << B <<"METHODS " << join_vector(key.methods, ", ") << Reset << std::endl;
	if (key.methods.size() > 0)
		std::cout << "FIRST METHOD " << key.methods[0] << std::endl;
	return "";
}