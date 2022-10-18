#include <unistd.h>
#include <string>

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
	return str;
}