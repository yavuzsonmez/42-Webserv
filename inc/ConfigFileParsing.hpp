#include <string>

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

		// gets thrown if the configuration file is faulty.
		class InvalidConfigurationFile : public std::exception {
			public:
				virtual const char* what() const throw() {
					return "configuration file is faulty";
				}
		};
	private:
		bool isGeneralFaultyFile( std::string &file_content );
};