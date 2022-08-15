#include "../../inc/DebuggerPrinter.hpp"
#include "../../inc/ServerBlock.hpp"

/**
 * Creates Server Block.
 */
ServerBlock::ServerBlock()
{
	this->debugger = debugger.getInstance();
	this->debugger.debug("Created ServerBlock");
}

ServerBlock::ServerBlock( const ServerBlock &src )
{

}

ServerBlock::~ServerBlock()
{

}

ServerBlock & ServerBlock::operator = (const ServerBlock &src) {
	return (*this);
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
			server_names.insert(server_names.end(), begin((*i).server_names), end((*i).server_names));;
	}
	return server_names;
}