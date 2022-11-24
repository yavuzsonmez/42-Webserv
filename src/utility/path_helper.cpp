#include "../../inc/debugger/DebuggerPrinter.hpp"
#include "../../inc/utility/utility.hpp"

/**
 * @brief Get the first location in path 
 * 
 * @param path 
 * @return std::string 
 */
std::string get_first_location_in_path(const std::string path)
{
	std::string first_location = path;
	size_t pos = first_location.find_first_of("/");
	if (pos != std::string::npos)
	{
		first_location = first_location.substr(0, pos);
	}
	return first_location;
}