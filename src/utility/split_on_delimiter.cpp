#include "../../inc/utility/utility.hpp"

/**
 * @brief returns a vector of strings split on the delimiter.
 * Will always return a vector of size one or size 2.
 * Never more
 * 
 * @param s string to split
 * @param delimiter char
 * @return std::vector<string> 
 */
std::vector<std::string> split_once_on_delimiter(std::string &s, char delimiter)
{
	std::vector<std::string> result;
	std::stringstream ss (s);
	std::string item;

	while (std::getline(ss, item, delimiter))
	{
		// appends the string to the last element of the vector instead adding a new element
		if (result.size() == 2)
		{
			result[1] += " ";
			result[1] += item;
		}
		else // adds a new element
		{
			result.push_back(item);
		}
	}
	return result;
}

/**
 * @brief returns a vector of strings split on the delimiter.
 * Can have multiple split
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

//// testing main and usage example
//int main() {
//	std::string s = "localhost localhost.localdomain test test2";
//	//std::vector<std::string> result = split_on_delimiter(s, ' ');
//	//for (auto item : result) {
//	//	std::cout << item << std::endl;
//	//}
//	std::vector<std::string> result2 = split_once_on_delimiter(s, ' ');
//	for (auto item : result2) {
//		std::cout << item << std::endl;
//	}
//	return 0;
//}