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

	std::string request = get_file_content("./sample/http_request_get");
	Request httpRequestGet(request);
	std::cout << httpRequestGet << std::endl;

	std::cout << request << std::endl;
}