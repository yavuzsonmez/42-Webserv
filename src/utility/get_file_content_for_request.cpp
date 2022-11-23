
#include "../../inc/utility/utility.hpp"

/**
 * @brief Get the file content for a path. 
 * 
 * @param path 
 * @return std::string 
 * @throw 404 if file could not be opened
 */
std::string get_file_content_for_request(std::string path)
{
	std::string content;
	int fd = open(path.c_str(), O_RDONLY);
	if (fd == -1)
		throw 404;
	char buffer[1024];
	int ret = 0;
	while ((ret = read(fd, buffer, 1024)) > 0)
		content.append(buffer, ret);
	close(fd);
	return content;
}

// int	main(int argc, char**argv)
// {
// 	(void)argc;
// 	std::cout << get_file_content(argv[1]) << std::endl;
// 	return (0);
// }