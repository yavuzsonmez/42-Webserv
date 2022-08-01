#include <string>
#include <vector>

/**
 * Defines the type of information a configuration key holds.
 * Lists all available configuration types.
 * Can be extended easily.
 */
enum ConfigurationKeyType { 
	INDEX, 
	SERVER_NAME,
    PORT,
    LOCATION,
    ROOT,
};

/**
 * Usage: ConfigurationKey(key, value);
 * 
 * @note The configuration key represents a single entry (or a body containing values) in the configuration file.
 *       It is to be used in an array, and parses the given string-value automatically
 *       to the correct type in the constructor and sets it own type.
 *       It can be nested, but only certain nestings are valid. (root in location block as example)
 * 
 * Currently, it supports following types:
 *  - server_name
 *  - port
 * 
 * To use it, split the key/value from the config_file from the first space and pass the first value as key and the rest as value.
 * Then it parses the value and by combining key and value it decides which type of configuration it is.
 * You can find the available configuration types in the ConfigurationKey.hpp in the enums.
 * 
 * If there is no type which fits the value/key, it throws an exception and the program stops.
 * 
 */
class ConfigurationKey {
    public:
        ConfigurationKey();
		ConfigurationKey( const ConfigurationKey &src );
		~ConfigurationKey();
		ConfigurationKey & operator = (const ConfigurationKey &src);

        // this is the initializer taking the key and the raw value from the configuration file
        ConfigurationKey(std::string key, std::string value);

        // Attributes

        std::string key;
        std::string value;

        std::vector <std::string> indexes; // indexes, if type is INDEX. sorted by relevance and position within the key.
    private:


}