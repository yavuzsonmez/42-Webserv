#include "../../inc/network/ServerSocket.hpp"
#include "../../inc/network/ClientSocket.hpp"
#include "../../inc/http/Response.hpp"
#include "../../inc/http/Process.hpp"
#include <sys/ioctl.h>

ServerSocket::ServerSocket(ServerBlock config, unsigned int address) : _config(config)
{
	_fd = socket(AF_INET, SOCK_STREAM, 0); //IPv4, TCP
	if (_fd < 0)
		throw SocketCreationError();

	_socket.sin_family = AF_INET;
	_socket.sin_port = htons(_config.getAllServerPorts().front()); //host byte order to network byte order
	_socket.sin_addr.s_addr = address;
	bzero(&(_socket.sin_zero), 8);

	if (bind(_fd, (struct sockaddr *)&_socket, sizeof(struct sockaddr_in)))
		throw SocketCreationError();

	if (listen(_fd, BACKLOG))
		throw SocketCreationError();

	processConnections();
}

ServerSocket::~ServerSocket(){}

int ServerSocket::getFileDescriptor() const { return _fd; }

void ServerSocket::processConnections()
{
	int forward;
	struct sockaddr_in clientSocket;
	socklen_t socketSize = sizeof(struct sockaddr_in);

	while (1)
	{
		forward = accept(_fd, (struct sockaddr *)&clientSocket, &socketSize);
		std::cout << "Request" << std::endl;

		// if accept return -1 throw error
		ClientSocket client(clientSocket);
		
		
		std::string	request_str;
		//char	buf[1000000];
		//bzero(buf, 1000000);
		// size_t	bytes = read(forward, buf, 1024);
		// while (bytes > 0)
		// {
		// 	request_str.resize(bytes);
		// 	size_t	i;
		// 	for (i = 0 ; i < bytes; i++)
		// 	{
		// 		char	*tmp = (char*)request_str.data();
		// 		tmp[i] = buf[i];
		// 	}
		// 	bzero(buf, 1024);
		// 	//bytes = read(forward, buf, 1024);
		// 	bytes = 0;
		// }

		// struct	pollfd	fds;
		// memset(&fds, 0, sizeof(fds));
		// fds.fd = forward;
		// fds.events = POLLIN;

		// int len;
		// while (poll(&fds, 1, 100) == 0){}
		// len = ioctl(forward, FIONREAD, &len);
		// request_str.resize(len);
		// read(forward, (char*)request_str.data(), len);
		int len = 0;
		while (!len && ioctl(forward, FIONREAD, &len) >= 0)

		request_str.resize(len);
		read(forward, (char*)request_str.data(), len);

		std::cout << request_str << std::endl;

		// read(forward, buf, 1000000);
		// write(STDOUT_FILENO, buf, 1000000);
		// std::string	request_str(buf);

		Request	request(request_str);
		
		Response response;
		Process	process(response, request, _config);
		// test url -> http://localhost:4242/?firstname=Yavuz&lastname=Sonmez&age=26&home=Heilbronn
		//response.test_cgi(_config);
		process.process_request();
		std::string httpResponse(response.get_response());

		int bytes_send;
		bytes_send = 0;
		// if bytes_send < len we have to handle it and keep send until everything was sent
		while (bytes_send < (int)httpResponse.length())
		{
			//if (bytes_send == -1)
				// if send return -1, throw error; + perror
			bytes_send = send(forward, httpResponse.c_str(), httpResponse.length(), 0);
			httpResponse.erase(0, bytes_send);
		}

		// std::string	body = response.get_body();
		// bytes_send = 0;
		// int	chunksize = 1048576;
		// std::string	hex = inttohex(chunksize);
		// std::cout << "lenght: " << (int)body.length() << std::endl;
		// // if bytes_send < len we have to handle it and keep send until everything was sent
		// while (bytes_send < (int)body.length())
		// {
		// 	//if (bytes_send == -1)
		// 		// if send return -1, throw error; + perror
			
		// 	//std::cout << "test" << std::endl;
		// 	while (poll(&fds, 1, 10) < 0){}
		// 	send(forward, (hex + "\r\n").c_str(), hex.length() + 2, 0);
		// 	//write(STDIN_FILENO, (char*)body.data() + bytes_send, 762);
		// 	while (poll(&fds, 1, 10) < 0){}
		// 	bytes_send += send(forward, (char*)body.data() + bytes_send, chunksize, 0);
		// 	std::cout << "bytes: " << bytes_send << std::endl;
		// }
		// send(forward, "0\r\n\r\n", 5, 0);

		// if (request.getMethod().first == POST)
		// {
		// 	int	content_length = 211623;		//from the request header content-length
		// 	char	*buff = (char*)calloc(sizeof(char), content_length);
		// 	std::cout <<  "test" << std::endl;
		// 	std::ofstream ofs("uploads/test.jpg", std::ofstream::out);
			
		// 	len = 0;
		// 	int	bytes = 0;
		// 	time_t	beginning = time(NULL);
		// 	while (len < content_length && difftime(time(NULL), beginning) <= 1)
		// 	{
		// 		bytes = read(forward, buff + len, 1024);
		// 		len += bytes;
		// 		if (bytes)
		// 			beginning = time(NULL);
		// 		std::cout << "len: " << len << std::endl;
		// 	}

		// 	std::cout << "write in file" << std::endl;
		// 	ofs.write(buff, content_length);
		// 	ofs.close();
		// 	free(buff);
		// 	std::cout << "finished" << std::endl;
		// 	response.set_protocol("HTTP/1.1");
		// 	response.set_status_code("200");
		// 	response.set_status_text("OK");
		// 	response.set_server(_config.getConfigurationKeysWithType(SERVER_NAME).front().server_names.front());
		// 	response.create_response();
		// 	bytes_send = 0;
		// 	// if bytes_send < len we have to handle it and keep send until everything was sent
		// 	std::cout << "test" << std::endl;
		// 	while (bytes_send < (int)response.get_response().length())
		// 	{
		// 		//if (bytes_send == -1)
		// 			// if send return -1, throw error; + perror
		// 		bytes_send = send(forward, response.get_response().c_str(), response.get_response().length(), 0);
		// 		response.get_response().erase(0, bytes_send);
		// 	}
		// }
		
		close(forward);
		//if close(server) //throw error; + perror
	}
}
