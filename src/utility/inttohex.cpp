# include <iomanip>
# include <sstream>
# include <iostream>

std::string	inttohex(int n)
{
	std::ostringstream stream;
	stream << std::hex << n;
	return stream.str();
}

// int main(int argc, char **argv)
// {
// 	(void)argc;
// 	int	n = atoi(argv[1]);
// 	std::cout << "int: " << n << std::endl;
// 	std::cout << "hex: " << inttohex(n).c_str() << std::endl;
// 	return (0);
// }