#include <sys/socket.h>
#include "../../inc/http/Response.hpp"
#include <sys/ioctl.h>
#include "../../inc/utility/utility.hpp"

/**
 * All the instant responses sent to the client without creating a client object, a process
 * to keep everything very simple and low overhead.
 * Do not use this on clients.
 */

/**
 * Sends a server unavailable message to the client (given filedescriptor)
 * After sending it here, do not forget to close the filedescriptor!
 * @param fd filedescriptor
 * @param serverblock so that we can receive the error page file
*/
int send_server_unavailable(int forward, ServerBlock serverblock)
{
	Response response;
	response.set_status_code(Service_Unavailable);
	response.set_protocol("HTTP/1.1");
	response.set_body("Service Unavailable");
	response.set_body(get_file_content(serverblock.getErrorPagePathForCode(503)));
	response.set_server("mostlyharmless2.com");
	response.set_content_type("text/html");
	response.set_content_length(to_str(response.get_body().length()));
	response.create_response();
	int result = send(forward, response.get_response().data(), response.get_body().length(), 0);
	return result;
}

/**
 * Sends a server timeout message to the client (given filedescriptor)
 * After sending it here, do not forget to close the filedescriptor!
 * @param fd filedescriptor
 * @param serverblock so that we can receive the error page file
*/
int send_client_timeout(int forward, ServerBlock serverblock)
{
	Response response;
	response.set_status_code(Service_Unavailable);
	response.set_protocol("HTTP/1.1");
	response.set_body("Service Unavailable");
	response.set_body(get_file_content(serverblock.getErrorPagePathForCode(500)));
	response.set_server("mostlyharmless2.com");
	response.set_content_type("text/html");
	response.set_content_length(to_str(response.get_body().length()));
	response.create_response();
	int result = send(forward, response.get_response().data(), response.get_body().length(), 0);
	return result;
}