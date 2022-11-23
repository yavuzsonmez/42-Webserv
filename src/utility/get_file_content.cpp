
#include "../../inc/utility/utility.hpp"

std::string get_file_content(std::string path)
{
	std::string content;
	int fd = open(path.c_str(), O_RDONLY);
	if (fd == -1)
		return content;
	char buffer[1024];
	int ret = 0;
	while ((ret = read(fd, buffer, 1024)) > 0)
		content.append(buffer, ret);
	close(fd);
	return content;
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