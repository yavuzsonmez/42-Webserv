#ifndef CONFIGFILEPARSING
# define CONFIGFILEPARSING

#include <string>
#include "../configuration_key/ServerBlock.hpp"
#include "../../inc/config_file/InvalidConfigurationFile.hpp"

/**
 * @brief Takes a config file and parses it into a config file configuration.
 * Sets the appropriate values in the config class.
 *
 * @note Before parsing with bool parseConfigFile, the ConfigFileParsing
 * will remove ALL comments (a comment starts with #) from the file.
 */
class ConfigFileParsing {
	public:
		ConfigFileParsing();
		ConfigFileParsing( const ConfigFileParsing &src );
		~ConfigFileParsing();
		ConfigFileParsing & operator = (const ConfigFileParsing &src);
		bool parseConfigFile( std::string &file_content );
		std::string getErrorPagePathForCode(int statuscode, ServerBlock serverBlock);

		// The server block vector which will provide the servers and their configurations
		std::vector<ServerBlock> serverBlocks;
	private:
		bool isGeneralFaultyFile( std::string &file_content );
		void determineConfigurationKeys( std::string &file_content );
		void addConfigurationKeyToCurrentServerBlock( ConfigurationKey &key );
		bool shouldSkipLineInConfigurationFile(std::string line, int firstNotWhiteSpacePosition);
		void printAllServerBlocks(std::vector<ServerBlock> &serverBlocks);
		void addConfigurationKeyToLocation( ConfigurationKey &key, ConfigurationKey keyToAdd );
		void addConfigurationKeyTypeToLocation(ConfigurationKeyType keyType, ConfigurationKey &keyToAdd);
		bool validateConfiguration();
		bool validationDuplicationCheck();
		bool isCurrentlyInLocationBlock;
		bool isCurrentlyInServerBlock;
		std::string getFallbackErrorPageForCode(int statuscode);
		int server_bracket_counter; // if is zero, we are currently not in a server block and config keys are not allowed
};

#endif