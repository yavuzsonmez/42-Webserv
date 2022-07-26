#include "inc/utility.hpp"
#include "inc/request.hpp"

int main(int argc, char **argv)
{
	if (argc != 2 || !check_config_file(&argv[1][0]))
	{
		std::cout << "./webserv [path/webserv.conf]" << std::endl;
		return (1);
	}

	//Request httpRequest_1;
	//std::cout << httpRequest << std::endl;


	std::cout << get_file_content("./http_request.sample") << std::endl;
	//Request httpRequest_2(get_file_content("./http_request.sample"));
	//std::cout << httpRequest << std::endl;
}