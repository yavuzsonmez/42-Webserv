#include "../../inc/utility/utility.hpp"
#include "../../inc/debugger/DebuggerPrinter.hpp"

/**
 * 
 * @brief Validates a string and checks if it is a valid url
 */
bool	validate_url(std::string url)
{
	USE_DEBUGGER;
	if (url.find("http://") == 0 || url.find("https://") == 0)
	{
		return true;
	}
	// check ip address for length and max value
	std::string ip = url.substr(0, url.find("/"));
	std::vector<std::string> ip_parts = split_on_delimiter(ip, '.');
	if (ip_parts.size() != 4)
	{
		debugger.error("Not correct number of ip parts");
		return false;
	}
	for (size_t i = 0; i < ip_parts.size(); i++)
	{
		if (ip_parts[i].length() > 3)
		{
			debugger.error("Ip address parts are too long!");
			return false;
		}
		if (stoi(ip_parts[i]) > 255)
		{
			debugger.error("ip address part is greater than 255");
			return false;
		}
	}
	return true;
}