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
 * @brief Check that every location block with a cgi_executable has a cgi_fileending and vice versa in a location block
 * 
 * @param serverBlocks
 */
bool checkIfCgiExecutableAndFileEndingAreSet(std::vector<ServerBlock> &serverBlocks)
{
	USE_DEBUGGER;
	std::vector<ServerBlock>::iterator i = serverBlocks.begin();
	std::vector<std::string> server_names;
	for (serverBlocks.begin(), serverBlocks.end(); i != serverBlocks.end(); ++i) {
		std::vector<ConfigurationKey> locationBlocksInServerBlock = (*i).getConfigurationKeysWithType(LOCATION); // go through all locations available
		std::vector<ConfigurationKey>::iterator j = locationBlocksInServerBlock.begin();
		for (locationBlocksInServerBlock.begin(), locationBlocksInServerBlock.end(); j != locationBlocksInServerBlock.end(); ++j) {
			ConfigurationKey locationBlock = *j;
			// either cgi_path and cgi_fileending are set or none of them are set
			if (locationBlock.cgi_path.empty() && !locationBlock.cgi_fileending.empty()) {
				return false;
			}
			if (!locationBlock.cgi_path.empty() && locationBlock.cgi_fileending.empty()) {
				return false;
			}
			return true;
		}
	}
	return true;
}


/**
 * @brief Iterate over all server blocks and makes sure each of them has the given keyType at least one.
 * 
 * @param serverBlocks
 * @param keyType
 * @return true if at least one is there, false if not
 */ 
bool keyExistsInEachServerBlock(std::vector<ServerBlock> &serverBlocks, ConfigurationKeyType keyType)
{
	USE_DEBUGGER;
	std::vector<ServerBlock>::iterator i = serverBlocks.begin();
	std::vector<std::string> server_names;

	for (serverBlocks.begin(), serverBlocks.end(); i != serverBlocks.end(); ++i) {
		std::vector<ConfigurationKey> configurationKeysPerBlock = (*i).getConfigurationKeysWithType(keyType);
		if (configurationKeysPerBlock.size() == 0) {
			return false;
		}
	}
	return true;
}


/**
 * @brief Iterate over all server blocks and all their location blocks and makes sure each of them has the given keyType at least once. If the requested keytype does not exist, there has to the alternative keytype.
 * 
 * @param serverBlocks
 * @param keyType
 * @param alternativeKeyType
 * @return true if at least one is there, false if not
 */ 
bool keyExistsOrAlternativeInEachLocationBlock(std::vector<ServerBlock> &serverBlocks, ConfigurationKeyType keyType, ConfigurationKeyType alternativeKeyType)
{
	USE_DEBUGGER;
	std::vector<ServerBlock>::iterator i = serverBlocks.begin();
	std::vector<std::string> server_names;

	for (serverBlocks.begin(), serverBlocks.end(); i != serverBlocks.end(); ++i) {
		std::vector<ConfigurationKey> locationBlocksInServerBlock = (*i).getConfigurationKeysWithType(LOCATION); // go through all locations available
		std::vector<ConfigurationKey>::iterator j = locationBlocksInServerBlock.begin();
		for (locationBlocksInServerBlock.begin(), locationBlocksInServerBlock.end(); j != locationBlocksInServerBlock.end(); ++j) {
			ConfigurationKey locationBlock = *j;
			std::vector<ConfigurationKeyType>::iterator k = locationBlock.nestedConfigurationKeyTypesinLocationBlock.begin();
			bool requestedConfigurationTypeIsAvailable = false;
			for (locationBlock.nestedConfigurationKeyTypesinLocationBlock.begin(), locationBlock.nestedConfigurationKeyTypesinLocationBlock.end(); k != locationBlock.nestedConfigurationKeyTypesinLocationBlock.end(); ++k) {
				if (*k == keyType) {
					requestedConfigurationTypeIsAvailable = true;
				} else if (*k == alternativeKeyType) {
					requestedConfigurationTypeIsAvailable = true;
				}
			}
			if (!requestedConfigurationTypeIsAvailable) {
				return false;
			}
		}
	}
	return true;
}


/**
 * @brief Iterate over all server blocks and all their location blocks and makes sure each of them has the given keyType at least once.
 * 
 * I hereby apologize for everyone working on this piece of crap.
 * It got quite complex quite fast. Basically I am just going down the hierarchy of the ServerBlocks into LocationBlocks in the NestedConfigurationTypeBlocks and check if the key exists in each of them.
 * 
 * @param serverBlocks
 * @param keyType
 * @return true if at least one is there, false if not
 */ 
bool keyExistsInEachLocationBlock(std::vector<ServerBlock> &serverBlocks, ConfigurationKeyType keyType)
{
	USE_DEBUGGER;
	std::vector<ServerBlock>::iterator i = serverBlocks.begin();
	std::vector<std::string> server_names;

	for (serverBlocks.begin(), serverBlocks.end(); i != serverBlocks.end(); ++i) {
		std::vector<ConfigurationKey> locationBlocksInServerBlock = (*i).getConfigurationKeysWithType(LOCATION); // go through all locations available
		std::vector<ConfigurationKey>::iterator j = locationBlocksInServerBlock.begin();
		for (locationBlocksInServerBlock.begin(), locationBlocksInServerBlock.end(); j != locationBlocksInServerBlock.end(); ++j) {
			ConfigurationKey locationBlock = *j;
			std::vector<ConfigurationKeyType>::iterator k = locationBlock.nestedConfigurationKeyTypesinLocationBlock.begin();
			bool requestedConfigurationTypeIsAvailable = false;
			for (locationBlock.nestedConfigurationKeyTypesinLocationBlock.begin(), locationBlock.nestedConfigurationKeyTypesinLocationBlock.end(); k != locationBlock.nestedConfigurationKeyTypesinLocationBlock.end(); ++k) {
				if (*k == keyType) {
					requestedConfigurationTypeIsAvailable = true;
				}
			}
			if (!requestedConfigurationTypeIsAvailable) {
				return false;
			}
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