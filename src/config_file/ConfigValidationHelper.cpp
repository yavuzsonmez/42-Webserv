#include "../../inc/config_file/ConfigFileParsing.hpp"
#include "../../inc/utility/utility.hpp"
#include "../../inc/debugger/DebuggerPrinter.hpp"

/**
 * All those functions prototypes can be found in utility.
 */

/**
 * 
 * @brief Insert vector b in vector a.
 * @param a reference vector to be inserted in
 * @param b vector to be inserted
 * @return std::vector<X> 
 */
template<typename X>
std::vector<X> append_vector(std::vector<X> &a, std::vector<X> b)
{
	a.insert(a.end(), b.begin(), b.end());
	return a;
}


/**
 * @brief Iterate over all server blocks and join ports together
 * @param server block reference
 * @return std::vector<unsigned int> ports 
 */
std::vector<unsigned int> getAllServerPortsFromAllServerBlocks(std::vector<ServerBlock> &serverBlocks) {
	std::vector<ServerBlock>::iterator i = serverBlocks.begin();
	std::vector<unsigned int> ports;

	for (serverBlocks.begin(), serverBlocks.end(); i != serverBlocks.end(); ++i) {
		append_vector(ports, (*i).getAllServerPorts());
	}
	return ports;
}

/**
 * @brief Iterate over all server blocks and join server names together
 * @param server block reference
 * @return std::vector<std::string> server_names
 */
std::vector<std::string> getAllServerNamesFromAllServerBlocks(std::vector<ServerBlock> &serverBlocks) {
	std::vector<ServerBlock>::iterator i = serverBlocks.begin();
	std::vector<std::string> server_names;

	for (serverBlocks.begin(), serverBlocks.end(); i != serverBlocks.end(); ++i) {
		append_vector(server_names, (*i).getAllServerNames());
	}
	return server_names;
}


/**
 * @brief Iterate over all server blocks and makes sure each of them has the given keyType jsut once.
 * 
 * @param serverBlocks
 * @param keyType
 * @return true if unique, false if not
 */ 
bool checkIfKeyIsUniqueInEachServerBlock(std::vector<ServerBlock> &serverBlocks, ConfigurationKeyType keyType)
{
	USE_DEBUGGER;
	std::vector<ServerBlock>::iterator i = serverBlocks.begin();
	std::vector<std::string> server_names;

	for (serverBlocks.begin(), serverBlocks.end(); i != serverBlocks.end(); ++i) {
		std::vector<ConfigurationKey> configurationKeysPerBlock = (*i).getConfigurationKeysWithType(keyType);
		if (configurationKeysPerBlock.size() > 1) {
			return false;
		}
	}
	return true;
}

/**
 * @brief Iterate over all server blocks and checks if there are duplicate error page paths
 * - general_error_page (one for each)
 * - not_found_error_page (one for each)
 * @param serverBlocks block reference
 * @return bool false or true when there are double pages
 */
bool checksIfAnyServerBlockHasDoubleErrorPages(std::vector<ServerBlock> &serverBlocks) {
	std::vector<ServerBlock>::iterator i = serverBlocks.begin();

	for (serverBlocks.begin(), serverBlocks.end(); i != serverBlocks.end(); ++i) {
		std::vector<ConfigurationKey> generalErrorPage = (*i).getConfigurationKeysWithType(GENERAL_ERROR_PAGE);
		std::vector<ConfigurationKey> notfoundErrorPage = (*i).getConfigurationKeysWithType(NOT_FOUND_ERROR_PAGE);
		
		if (generalErrorPage.size() > 1 || notfoundErrorPage.size() > 1) {
			return true;
		}
	}
	return false;
}