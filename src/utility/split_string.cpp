#include <string>
#include <vector>
#include <sstream>
#include <iostream>

std::vector<std::string>	split_string(std::string str, char delim)
{
	std::vector<std::string>	vector;
	std::stringstream	ss(str);
	std::string	token;

	while (std::getline(ss, token, delim))
	{
		vector.push_back(token);
	}
	return vector;
}

// int main(void)
// {
// 	std::string test = "name=Paul&age=42&home=Heilbronn";
// 	char	delimiter = '&';

// 	std::vector<std::string> vec = split_string(test, delimiter);
// 	std::vector<std::string>::iterator	it;
// 	for (it = vec.begin(); it != vec.end(); it++)
// 	{
// 		std::cout << *it << std::endl;
// 	}
// 	return 0;
// }