#ifndef SERVER_BLOCK
# define SERVER_BLOCK

#include "ConfigurationKey.hpp"


/**
 * SERVERBLOCK
 *
 * represents a server block in configuration file.
 * Is being built by the configuration key parser.
 *
 * Holds multiple configuration keys
 *
 * Also has functions to easily receive all ports and server names and indexes (ordered)
 */
class ServerBlock
{
    public:
        ServerBlock();
        ServerBlock(ServerBlock const&);
		~ServerBlock();
		ServerBlock & operator = (const ServerBlock &src);
        std::vector<ConfigurationKey> configurationKeys;
        std::vector<std::string> getAllServerNames();
        std::vector<unsigned int> getAllServerPorts();
        std::vector<std::string> getAllIndexes();
        std::string getCgiPath();
        std::string getCgiFileEnding();
        void addConfigurationKey(ConfigurationKey &configurationKey);
		std::vector<ConfigurationKey> getConfigurationKeysWithType(ConfigurationKeyType type);
};

 #endif