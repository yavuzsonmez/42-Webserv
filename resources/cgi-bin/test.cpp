#include <iostream>
#include "../inc/utility.hpp"

int	main(int argc, char **argv)
{
	if (argc <= 1)
		std::cout << "no parameter" <<  std::endl;
	else
	{
		std::cout << get_file_content(argv[1]) << std::endl;
	}
	return (0);
}