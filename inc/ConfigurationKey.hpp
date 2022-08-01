#include <string>
#include <vector>

/**
 * All keys which can be used in the configuration file are defined here.
 */

# define	KEY_LOCATION	        "location"
# define	KEY_INDEX	            "index"
# define	KEY_ROOT	            "root"
# define	KEY_SERVER_NAMES	    "server_names"
# define	KEY_LISTEN	            "listen"

/**
 * Defines the type of information a configuration key holds.
 * Lists all available configuration types.
 * Can be extended easily.
 */
enum ConfigurationKeyType { 
	INDEX, 
	SERVER_NAME,
    LISTEN,
    LOCATION,
    ROOT,
    INVALID
};

/**
 * The internal keypair used within the class to transport key and value. This is being isolated to avoid confusion between attribute key/value and
 * internal handling of key and value
 */
typedef std::pair <std::string, std::string>	internal_keyvalue;

/**
 * Usage: ConfigurationKey(key, value);
 * 
 * @note The configuration key represents a single entry (or a body containing values) in the configuration file.
 *       It is to be used in an array, and parses the given string-value automatically
 *       to the correct type in the constructor and sets it own configurationType.
 *       It can be nested, but only certain nestings are valid. (root in location block as example)
 * 
 * Currently, it supports following types:
 *  - server_name
 *  - port
 * 
 * To use it, split the key/value from the config_file from the first space and pass the first value as key and the rest as value.
 * Then it parses the value and by combining key and value it decides which configurationType it is.
 * You can find the available configuration types in the ConfigurationKey.hpp in the enums.
 * 
 * If there is no type which fits the value/key, it throws an exception and the program stops.
 * 
 * @note Internal handling in ConfigurationKey class is done with internal_keyvalue Pair typedef for readabiliy.
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

        // General Attributes

        std::string key;
        std::string value;

        // Configuration type
        ConfigurationKeyType configurationType;

        // Attributes of the configuration types
        std::vector <std::string> indexes; // indexes, if type is INDEX. sorted by relevance and position within the key.
        std::string root; // returns the path of the root
        std::string location; // returns the locationpath of the location
        std::vector <unsigned int> ports; // returns the ports which are being listened to by the listener handler
        std::vector<ConfigurationKey> nestedConfigurationKey; // describes the properties within the location block
    private:
        ConfigurationKeyType detectConfigurationType(internal_keyvalue raw);
        bool isServerNameKeyType(internal_keyvalue raw);
};