# include "../../inc/http/Location.hpp"

Location::Location(void)
{

}

Location::Location(ServerBlock config)
{
	_root = config.getConfigurationKeysWithType(ROOT).front().root;
	_index = config.getConfigurationKeysWithType(INDEX).front().indexes.front();
}

Location::Location(ConfigurationKey key)
{
	_root = key.root;
	_index = key.indexes.front();
}

Location::~Location(void)
{

}

