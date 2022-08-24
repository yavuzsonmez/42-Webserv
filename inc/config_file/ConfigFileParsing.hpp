#ifndef CONFIGFILEPARSING
# define CONFIGFILEPARSING

#include <string>
#include "../configuration_key/ServerBlock.hpp"

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

		// The server block vector which will provide the servers and their configurations
		std::vector<ServerBlock> serverBlocks;

		// gets thrown if the configuration file is faulty.
		class InvalidConfigurationFile : public std::exception {
			public:
				virtual const char* what() const throw() {
					return "configuration file is faulty";
				}
		};
	private:
		bool isGeneralFaultyFile( std::string &file_content );
		void determineConfigurationKeys( std::string &file_content );
		void addConfigurationKeyToCurrentServerBlock( ConfigurationKey &key );
		bool shouldSkipLineInConfigurationFile(std::string line, int firstNotWhiteSpacePosition);
		void printAllServerBlocks(std::vector<ServerBlock> &serverBlocks);
};

#endif