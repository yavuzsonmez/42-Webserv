#ifndef UTILITY_HPP
# define UTILITY_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <string>
# include <cctype>
# include <algorithm>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <iterator>
# include <stack>
# include <vector>
# include <map>
# include <utility>
# include <set>
# include <unistd.h>
# include "../config_file/ConfigFileParsing.hpp"

void strip_from_str(std::string &file_content, const char start, const char end);
void upper_str(std::string &str);
void lower_str(std::string &str);
bool is_file_accessible(std::string const &path);
std::string get_file_content(std::string path);
bool check_config_file(std::string const &path);
std::string get_file_name(std::string const &path);
std::string filter_characters(std::string characters);
bool	validate_parantheses(std::string parantheses);
std::vector<std::string> split_on_delimiter(std::string &s, char delimiter);
std::vector<std::string> split_once_on_delimiter(std::string &s, char delimiter);
std::string convert_configuration_key_type(ConfigurationKeyType keyType);
std::string printOutNestedKeysFromLocationBlocks(ConfigurationKey &key);
std::string join_vector(std::vector<std::string> &vec, std::string delimiter);
std::string printKeyValueColored(std::string key, std::string name);
std::vector<unsigned int> getAllServerPortsFromAllServerBlocks(std::vector<ServerBlock> &serverBlocks);
std::vector<std::string> getAllServerNamesFromAllServerBlocks(std::vector<ServerBlock> &serverBlocks);
std::string trim_whitespaces(std::string str);

/**
 * @brief converts any type into a string
 * 
 * @tparam T 
 * @param input 
 * @return std::string 
 */
template <typename T>
std::string	to_str(T input)
{
	std::stringstream	ss;
	ss << input;
	return ss.str();
}

/**
 * @brief converts a map of any key-value type into an array of char*
 * 
 * @tparam key 
 * @tparam value 
 * @param map 
 * @return char** 
 */
template <typename key, typename value>
char **map_to_array(std::map<key, value> &map)
{
	typename std::map<key, value>::iterator	it;
	
	char **array = new char*[sizeof(char*) * (map.size() + 1)];
	//char	*array[map.size() + 1];
	int	i = 0;
	for (it = map.begin(); it != map.end(); ++it)
	{
		array[i] = strdup((to_str(it->first) + "=" + to_str(it->second)).c_str());
		++i;
	}
	array[i] = NULL;
	return array;
}

/**
 * @brief converts a vector of any type into an array of char*
 * 
 * @tparam T 
 * @param vector 
 * @return char** 
 */
template <typename T>
char **vec_to_array(std::vector<T> &vector)
{
	typename std::vector<T>::iterator	it;
	
	char **array = new char*[sizeof(char*) * (vector.size() + 1)];
	//char	*array[vector.size() + 1];
	int	i = 0;
	for (it = vector.begin(); it != vector.end(); ++it)
	{
		array[i] = strdup(to_str(*it).c_str());
		++i;
	}
	array[i] = NULL;
	return array;
}


template<typename X>
bool vector_has_duplicate_element(std::vector<X> v)
{
	std::set<X> s(v.begin(), v.end());
	return s.size() != v.size();

}
#endif