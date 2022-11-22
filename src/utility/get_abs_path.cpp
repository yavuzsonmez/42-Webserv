#include <unistd.h>
#include <string>

/**
 * @brief Get the absolute path of a file WITHOUT any arguments if they were provided (e.g. index.php?arg1=1&arg2=2)
 * 
 * @param path 
 * @return std::string 
 */
std::string	get_abs_path(const std::string & path)
{
	std::string str;
	char * pwd = getcwd(NULL, 0);
	if (pwd)
	{
		str = pwd;
		free(pwd);
	}
	str += path.substr(1);
	// remvoe arguments from str
	size_t pos = str.find('?');
	if (pos != std::string::npos)
		str.erase(pos);
	return str;
}