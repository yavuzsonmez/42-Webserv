#include <sys/socket.h>
#include "../../inc/http/Response.hpp"
#include <sys/ioctl.h>
#include "../../inc/utility/utility.hpp"

/**
 * Sends a server unavailable message to the client (given filedescriptor)
 * After sending it here, do not forget to close the filedescriptor!
 * @param fd filedescriptor
*/
int send_server_unavailable(int forward)
{
	Response response;
	response.set_status_code(Service_Unavailable);
	response.set_protocol("HTTP/1.1");
	response.set_body("Service Unavailable");
	response.set_server("Petroulette Web Server");
	response.set_content_length(to_str(response.get_body().length()));
	response.create_response();
	std::cout << response.get_response() << std::endl;
	int result = send(forward, response.get_response().data(), response.get_body().length(), 0);
	return result;
}