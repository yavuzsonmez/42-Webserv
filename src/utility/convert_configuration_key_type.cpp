#include "../../inc/configuration_key/ConfigurationKey.hpp"
#include "../../inc/utility/utility.hpp"
#include "../../inc/utility/colors.hpp"

/**
 * This file contains mainly debugging printing functions.
 */

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
	if (keyType == CGI_EXECUTABLE_PATH) {
		return "CGI PATH";
	}
	if (keyType == NOT_FOUND_ERROR_PAGE) {
		return "NOT_FOUND_ERROR_PAGE";
	}
	if (keyType == GENERAL_ERROR_PAGE) {
		return "GENERAL_ERROR_PAGE";
	}
	if (keyType == POST_MAX_SIZE) {
		return "POST MAX SIZE";
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
	std::string intendation = "   ";
	std:: cout << intendation;
	std::cout << B << "->" << Reset << " ROOT " << key.root << std::endl;
	std:: cout << intendation;
	std:: cout << intendation;
	std::cout << B << "->" << Reset << " METHODS " << join_vector(key.methods, ", ")  << std::endl;
	std:: cout << intendation;
	std:: cout << intendation;
	std::cout << B << "->" << Reset << " INDEX " << join_vector(key.indexes, ", ")  << std::endl;
	std:: cout << intendation;
	std:: cout << intendation;
	std::cout << B << "->" << Reset << " CGI_PATH " << key.cgi_path  << std::endl;
	std:: cout << intendation;
	std:: cout << intendation;
	std::cout << B << "->" << Reset << " CGI ENDING " << key.cgi_fileending  << std::endl;
	if (key.directory_listing) {
		std:: cout << intendation;
		std:: cout << intendation;
		std::cout << B << "->" << Reset << " DIRECTORY LISTING " << key.directory_listing  << std::endl;
	}
	return "";
}

/**
 * @brief prints out colored key and value
 * 
 * @param key to print
 * @param name to print
 * @return std::string 
 */
std::string printKeyValueColored(std::string key, std::string name) {
	std::cout << B << key << Reset << " " << "|" << name << "|" << std::endl;
	return "";
}