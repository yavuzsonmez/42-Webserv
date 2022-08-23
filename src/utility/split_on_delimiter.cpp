#include "../../inc/utility.hpp"
#include <iostream>
#include <sstream>
#include <vector>

/**
 * @brief returns a vector of strings split on the delimiter
 * 
 * @param s string to split
 * @param delimiter char
 * @return std::vector<string> 
 */
std::vector<std::string> split_on_delimiter(std::string &s, char delimiter)
{
	std::vector<std::string> result;
	std::stringstream ss (s);
	std::string item;

	for (std::string item; std::getline (ss, item, delimiter); result.push_back (item));
	return result;
}

// testing main and usage example
//int main() {
//	std::string s = "localhost localhost.localdomain test";
//	std::vector<std::string> result = split_on_delimiter(s, ' ');
//	for (auto item : result) {
//		std::cout << item << std::endl;
//	}
//	return 0;
//}