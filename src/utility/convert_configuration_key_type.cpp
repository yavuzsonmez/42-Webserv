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
	return "UNKNOWN";
}