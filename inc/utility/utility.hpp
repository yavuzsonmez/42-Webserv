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
# include "../configuration_key/ServerBlock.hpp"

void strip_from_str(std::string &file_content, const char start, const char end);
void upper_str(std::string &str);
void lower_str(std::string &str);
bool is_file_accessible(std::string const &path);
std::string	get_abs_path(const std::string & path);
std::string get_file_content(std::string path);
std::string get_file_content_for_request(std::string path);
bool check_config_file(std::string const &path);
bool isalphastring(std::string str);
std::string get_file_name(std::string const &path);
std::string filter_characters(std::string characters);
bool no_whitespace_between(std::string str);
bool	validate_parantheses(std::string parantheses);
std::vector<std::string> split_on_delimiter(std::string &s, char delimiter);
std::vector<std::string> split_once_on_delimiter(std::string &s, char delimiter);
std::string convert_configuration_key_type(ConfigurationKeyType keyType);
std::string printOutNestedKeysFromLocationBlocks(ConfigurationKey &key);
std::string join_vector(std::vector<std::string> &vec, std::string delimiter);
std::string printKeyValueColored(std::string key, std::string name);
std::vector<unsigned int> getAllServerPortsFromAllServerBlocks(std::vector<ServerBlock> &serverBlocks);
std::vector<std::string> getAllServerNamesFromAllServerBlocks(std::vector<ServerBlock> &serverBlocks);
bool checkIfKeyIsUniqueInEachServerBlock(std::vector<ServerBlock> &serverBlocks, ConfigurationKeyType keyType);
std::string trim_whitespaces(std::string str);
std::string	inttohex(int n);
bool isnumberstring(std::string str);
bool checksIfAnyServerBlockHasDoubleErrorPages(std::vector<ServerBlock> &serverBlocks);
bool keyExistsInEachServerBlock(std::vector<ServerBlock> &serverBlocks, ConfigurationKeyType keyType);
bool	isnumberstring(std::string str);
bool	checksIfAnyServerBlockHasDoubleErrorPages(std::vector<ServerBlock> &serverBlocks);
bool	validate_url(std::string url);
bool keyExistsInEachLocationBlock(std::vector<ServerBlock> &serverBlocks, ConfigurationKeyType keyType);
bool checkIfCgiExecutableAndFileEndingAreSet(std::vector<ServerBlock> &serverBlocks);
std::string remove_dot_if_first_character_is_dot(std::string to_edit);

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

template<typename T1, typename T2>
int	find_vector(std::vector<T1> &vector, T2 element)
{
	typename std::vector<T1>::iterator	it;
	for (it = vector.begin(); it != vector.end(); it++)
	{
		if (*it == element)
			return it - vector.begin();
	}
	return -1;
}

#endif