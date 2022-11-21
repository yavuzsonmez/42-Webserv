
#include "../../inc/utility/utility.hpp"

std::string get_file_content(std::string path)
{
	std::ifstream f(path); //taking file as inputstream
	std::string str;

	if(f) {
		std::ostringstream ss;
		ss << f.rdbuf(); // reading data
		str = ss.str();
	}
	return (str);
}

/**
 * @brief Get the file content. Caches the file content, if it was already read.
 * 
 * @param path 
 * @return std::string 
 */
std::string get_file_content_cached(std::string path)
{
	static std::map<std::string, std::string> file_content_cache;

	if (file_content_cache.find(path) == file_content_cache.end()) {
		file_content_cache[path] = get_file_content(path);
	}
	return file_content_cache[path];
}

// int	main(int argc, char**argv)
// {
// 	(void)argc;
// 	std::cout << get_file_content(argv[1]) << std::endl;
// 	return (0);
// }