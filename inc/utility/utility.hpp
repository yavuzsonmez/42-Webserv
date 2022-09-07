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
# include <utility>
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

#endif