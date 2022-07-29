#include "../../inc/ConfigFileParsing.hpp"
#include "../../inc/utility.hpp"

ConfigFileParsing::ConfigFileParsing()
{
	
}

ConfigFileParsing::ConfigFileParsing( const ConfigFileParsing &src )
{

}

ConfigFileParsing::~ConfigFileParsing()
{

}

ConfigFileParsing & ConfigFileParsing::operator = (const ConfigFileParsing &src) {
	return (*this);
}

/**
 * @brief Parses the given content from the content file
 * @note Removes all the comments from a configuration file. (a comment starts with #)
 * @param file_content 
 * @return true 
 * @return false or an throw exception if the configuration file is faulty.
 */
bool ConfigFileParsing::parseConfigFile( std::string &file_content ) {
	std::cout << "Cleaning configuration file..." << file_content << std::endl;
	strip_from_str(file_content, '#', '\n');
	std::cout << "After cleaning configuration file..." << std::endl << file_content << std::endl;
	if (isGeneralFaultyFile(file_content)) {
		return false;
	}
	return true;
}

/**
 * @brief Checks if the file is empty, does not have valid parantheses/brackets, contains invalid symbols or
 * does not have any parantheses (body) in it.
 * 
 * @param file_content 
 * @return true if file is faulty
 */
bool ConfigFileParsing::isGeneralFaultyFile( std::string &file_content ) {
	if (file_content.empty() == true) {
		throw true;
	}
	// add parantheses check. currently paranthesis check is causing a crash.
	return false;
}