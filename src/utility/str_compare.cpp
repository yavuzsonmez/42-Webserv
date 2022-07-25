
#include "../../inc/utility.hpp"


// return true if 2 strings are exactly equal
// to compare the name of the config file provided by the user to the one we accept
bool str_compare(std::string const &s1, std::string const &s2)
{
	if (!s1.compare(s2))
		return (true);
	return (false);
}

int	main(int argc, char**argv)
{
	std::string config_file = "webserv.conf";
	std::string norm = "webserv.conf";

	std::cout << str_compare(config_file, norm) << std::endl;
	return (0);
}