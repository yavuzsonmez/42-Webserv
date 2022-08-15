
#ifndef UTILITY_HPP
# define UTILITY_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <string>
# include <cctype>
# include <algorithm>
# include <map>

void strip_from_str(std::string &file_content, const char start, const char end);
void upper_str(std::string &str);
void lower_str(std::string &str);
bool is_file_accessible(std::string const &path);
std::string get_file_content(std::string path);
bool check_config_file(std::string const &path);
std::string get_file_name(std::string const &path);

template <typename T>
std::string	to_str(T input)
{
	std::stringstream	ss;
	ss << input;
	return ss.str();
}

template <typename key, typename value>
char **map_to_array(std::map<key, value> map)
{
	typename std::map<key, value>::iterator	it;
	
	char	*array[map.size() + 1];
	int	i = 0;
	for (it = map.begin(); it != map.end(); ++it)
	{
		array[i] = strdup((to_str(it->first) + "=" + to_str(it->second)).c_str());
		++i;
	}
	array[i] = NULL;
	return array;
}

# endif