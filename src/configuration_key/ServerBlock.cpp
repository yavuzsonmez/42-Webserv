#include "../../inc/configuration_key/ServerBlock.hpp"
#include "../../inc/debugger/DebuggerPrinter.hpp"
#include "../../inc/utility/utility.hpp"

/**
 * Creates Server Block.
 */
ServerBlock::ServerBlock()
{
	USE_DEBUGGER;
	debugger.debug("Created ServerBlock");
}

ServerBlock::ServerBlock( const ServerBlock &src )
{
	this->configurationKeys = src.configurationKeys;
	this->serverIndex = src.serverIndex;
}

ServerBlock::~ServerBlock()
{
	
}

ServerBlock & ServerBlock::operator = (const ServerBlock &src) {
	configurationKeys = src.configurationKeys;
	serverIndex = src.serverIndex;
	return (*this);
}

/**
 * @brief Add another configuration key to this server block.
 * Server block configuration keys are sorted and their order cannot be changed.
 * 
 * @param configurationKey configuraton key to be added
 * 
 * @returns nothing
 */
void ServerBlock::addConfigurationKey(ConfigurationKey &configurationKey) {
	this->configurationKeys.push_back(configurationKey);
}

/**
 * @brief Get all configuration keys with the requested type
 * @param type type of configuration keys to be returned
 */
std::vector<ConfigurationKey> ServerBlock::getConfigurationKeysWithType(ConfigurationKeyType type) {
	std::vector<ConfigurationKey> keys;
	for (int i = 0; i < (int) this->configurationKeys.size(); i++) {
		if (this->configurationKeys[i].configurationType == type) {
			keys.push_back(this->configurationKeys[i]);
		}
	}
	return keys;
}

/**
 * @brief returns the general cgi path of the server block
 * 
 * @return std::string 
 */
std::string ServerBlock::getCgiPath() {
	std::vector<ConfigurationKey> configKeys = this->getConfigurationKeysWithType(CGI_EXECUTABLE_PATH);
	if (configKeys.size() == 0) {
		return "";
	}
	std::string cgi_path = configKeys[0].value;
	return cgi_path;
}

/**
 * @brief returns the general cgi file ending of the server block
 * 
 * @return std::string 
 */
std::string ServerBlock::getCgiFileEnding() {
	std::vector<ConfigurationKey> configKeys = this->getConfigurationKeysWithType(CGI_FILEENDING);
	if (configKeys.size() == 0) {
		return "";
	}
	std::string cgi_fileending = configKeys[0].value;
	return remove_dot_if_first_character_is_dot(cgi_fileending);
}

/**
 * Returns all ports in the correct order
 *
 * - Iterates over all vectors
 * - adds the member of the vector ports of the configuration key
 * 	 to the result ports of getAllServerPorts
 */
std::vector<unsigned int> ServerBlock::getAllServerPorts()
{

	std::vector<ConfigurationKey>::iterator i = this->configurationKeys.begin();
	std::vector<unsigned int> ports;

	for (this->configurationKeys.begin(), this->configurationKeys.end(); i != this->configurationKeys.end(); ++i) {
		if ((*i).configurationType == LISTEN)
			ports.insert(ports.end(), (*i).ports.begin(), (*i).ports.end());;
	}
	return ports;
}

/**
 * Returns all indexes in the correct order
 *
 * - Iterates over all vectors
 * - adds the member of the vector indexes of the configuration key
 * 	 to the result indexes of getAllIndexes
 */
std::vector<std::string> ServerBlock::getAllIndexes() {

	std::vector<ConfigurationKey>::iterator i = this->configurationKeys.begin();
	std::vector<std::string> indexes;

	for (this->configurationKeys.begin(), this->configurationKeys.end(); i != this->configurationKeys.end(); ++i) {
		if ((*i).configurationType == SERVER_NAME)
			indexes.insert(indexes.end(), (*i).indexes.begin(), (*i).indexes.end());;
	}
	return indexes;
}

/**
 * Returns all server names in the correct order
 *
 * - Iterates over all vectors
 * - adds the member of the vector server_names of the configuration key
 * 	 to the result server_names of getAllServerNames
 */
std::vector<std::string> ServerBlock::getAllServerNames() {

	std::vector<ConfigurationKey>::iterator i = this->configurationKeys.begin();
	std::vector<std::string> server_names;

	for (this->configurationKeys.begin(), this->configurationKeys.end(); i != this->configurationKeys.end(); ++i) {
		if ((*i).configurationType == SERVER_NAME)
			server_names.insert(server_names.end(), (*i).server_names.begin(), (*i).server_names.end());
	}
	return server_names;
}


/**
 * @brief Fallback error pages if the given error page does not exist.
 * 
 * @param statuscode 
 * @param serverBlock
 * @return std::string 
 */
std::string ServerBlock::getFallbackErrorPageForCode(int statuscode)
{
	switch (statuscode)
	{
		case 404:
			return "resources/default_pages/404_default.html";
			break;
		case 405:
			return "resources/default_pages/405_default.html";
			break;
		case 500:
			return "resources/default_pages/500_default.html";
			break;
		case 501:
			return "resources/default_pages/501_default.html";
			break;
		case 502:
			return "resources/default_pages/502_default.html";
			break;
		case 504:
			return "resources/default_pages/504_default.html";
			break;
		case 503:
			return "resources/default_pages/503_default.html";
			break;
		case 413:
			return "resources/default_pages/413_default.html";
			break;
		default:
			return "resources/default_pages/500_default.html";
			break;
	}
}
/**
 * @brief Returns the error page path to the given error code
 * - Handles costum error pages and returns only valid paths that are available
 * 
 * TODO: add a caching for this so we do not access the file all the time during server runtime
 * @param statuscode http error code
 * @param serverBlock to check in
 * @return std::string path to error code
 * 
 * TODO: Replace GENERAL_ERROR_PAGE with the pages which are not customizable by the user
 */
std::string ServerBlock::getErrorPagePathForCode(int statuscode)
{
	std::string path_to_file;

	path_to_file = "./resources/default_pages/404_default.html";

	if (statuscode == 404) {
		path_to_file = "./resources/default_pages/404_default.html";
	}
	if (statuscode == 500) {
		path_to_file = "./resources/default_pages/500_default.html";
	}
	if (statuscode == 503) {
		path_to_file = "./resources/default_pages/503_default.html";
	}
	if (statuscode == 401) {
		path_to_file = "./resources/default_pages/401_default.html";
	}
	if (statuscode == 413) {
		path_to_file = "./resources/default_pages/413_default.html";
	}

	switch (statuscode)
	{
		case 404:
			if (getConfigurationKeysWithType(NOT_FOUND_ERROR_PAGE).size() != 0)
				path_to_file = getConfigurationKeysWithType(NOT_FOUND_ERROR_PAGE).front().not_found_error_page_path;
			break;
		case 500:
			if (getConfigurationKeysWithType(GENERAL_ERROR_PAGE).size() != 0)
				path_to_file = getConfigurationKeysWithType(GENERAL_ERROR_PAGE).front().general_error_page_path;
			break;
		case 503:
			if (getConfigurationKeysWithType(NOT_AVAILABLE_PAGE).size() != 0)
				path_to_file = getConfigurationKeysWithType(NOT_AVAILABLE_PAGE).front().not_available_page_path;
		break;
	}
	if (is_file_accessible(path_to_file))
		return path_to_file;
	
	return getFallbackErrorPageForCode(statuscode);
}