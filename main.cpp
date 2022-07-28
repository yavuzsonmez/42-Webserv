#include "inc/utility.hpp"
#include "inc/request.hpp"

int main(int argc, char **argv)
{
	if (argc != 2 || !check_config_file(&argv[1][0]))
	{
		std::cout << "./webserv [path/webserv.conf]" << std::endl;
		return (1);
	}


	std::string get = get_file_content("./sample/http_request_get");
	std::string post = get_file_content("./sample/http_request_post");
	std::string del = get_file_content("./sample/http_request_delete");
	std::string unknwn = get_file_content("./sample/http_request_delete");

	//Request httpRequestPost(post);
	Request httpRequestGet(get);
	//Request httpRequestDelete(del);
	//Request httpRequestDelete(unknwn);

	std::cout << httpRequestGet << std::endl;
	std::cout << get << std::endl;

}