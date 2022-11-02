#include "../../inc/network/ServerSocket.hpp"
#include "../../inc/http/Response.hpp"
#include "../../inc/http/Process.hpp"
#include <sys/ioctl.h>




ServerSocket::ServerSocket(ServerBlock config, unsigned int address) : _config(config)
{
	std::vector<struct sockaddr_in>::iterator	so;
	listeningSockets = _config.getAllServerPorts().size();
	_sockets.resize(listeningSockets);
	int	i = 0;
	for (so = _sockets.begin(); so != _sockets.end(); so++, i++)
	{
		(*so).sin_family = AF_INET;
		(*so).sin_port = htons(_config.getAllServerPorts()[i]);
		(*so).sin_addr.s_addr = address;
		bzero(&((*so).sin_zero), 8);
	}

	const int	enable = 1;
	_fds.resize(_sockets.size());
	std::vector<int>::iterator	fd;
	for (fd = _fds.begin(); fd != _fds.end(); fd++)
	{
		(*fd) = socket(AF_INET, SOCK_STREAM, 0); //IPv4, TCP
		if (*fd < 0)
			throw SocketCreationError();
		fcntl(*fd, F_SETFL, fcntl(*fd, F_GETFL, 0) | O_NONBLOCK);
		setsockopt(*fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
	}

	for (fd = _fds.begin(), so = _sockets.begin(); fd != _fds.end(); fd++, so++)
	{
		if (bind(*fd, (struct sockaddr *)&(*so), sizeof(struct sockaddr_in)))
			throw SocketCreationError();
		if (listen(*fd, BACKLOG))
			throw SocketCreationError();
	}

	processConnections();
}

ServerSocket::~ServerSocket(){}

//int ServerSocket::getFileDescriptor() const { return _fd; }

/**
 * @brief Handles connections 
 */
void ServerSocket::processConnections()
{
	int forward;
	struct sockaddr_in clientSocket;
	socklen_t socketSize = sizeof(struct sockaddr_in);

	std::string	request_str;
	int len;
	int	bytes;
	int	position;
	
	std::vector<pollfd> pollfds;
	pollfds.resize(3);

	std::vector<int>::iterator	it = _fds.begin();
	std::vector<int>::iterator	ite = _fds.end();
	for (unsigned int i = 0; it != ite; ++it, ++i)
	{
		pollfds[i].fd = *it;
		pollfds[i].events = POLLIN;
		pollfds[i].revents = 0;
	}
	while (1)
	{
		if (poll((struct pollfd *)(pollfds.data()), _fds.size(), -1) < 1)
			std::cout << "An error occured when polling.";
		for (unsigned long i = 0; i < _fds.size(); ++i)
		{
			if (i < listeningSockets)
			{
				if (pollfds[i].revents == POLLIN)
				{
					struct pollfd tmp;
					forward = accept(pollfds[i].fd, (struct sockaddr *)&clientSocket, &socketSize);
					fcntl(forward, F_SETFL, fcntl(forward, F_GETFL, 0) | O_NONBLOCK);
					tmp.fd = forward;
					tmp.events = POLLIN;
					tmp.revents = 0;
					pollfds.push_back(tmp);
					_clients.insert(std::pair<int, ClientSocket>(forward, ClientSocket(clientSocket, _config, forward)) );
				}
			}
			else
			{
				if (pollfds[i].revents == POLLIN)
					//read what is readable
				{
					try {
						if (!_clients[pollfds[i].fd].read_in_buffer())
							pollfds[i].events = POLLOUT;
					}
					catch (std::string error) {

						if (error == Request_Timeout)
							pollfds[i].events = POLLOUT;
					}
				}
			 	else if (pollfds[i].revents == 	POLLOUT)
				{
					if (!_clients[pollfds[i].fd].write_from_buffer())
					{
						_clients.erase(pollfds[i].fd);
						std::vector<pollfd>::iterator	del = pollfds.begin() + i;
						pollfds.erase(del);
					}
				}
			 		//if the response was build write the response to the fd
			}
		}

		//

	// 	std::cout << "Request" << std::endl;

	// 	// if accept return -1 throw error
		
	// 	len = 1024;
	// 	bytes = 0;
	// 	position = 0;
	// 	request_str.resize(len);
	// 	bytes = read(forward, (char*)request_str.data(), len);
	// 	while (bytes == len)
	// 	{
	// 		position += bytes;
	// 		request_str.resize(request_str.size() + len);
	// 		bytes = read(forward, (char*)request_str.data() + position, len);
	// 	}

	// 	std::cout << request_str << std::endl;

	// 	Request	request(request_str);
	// 	request_str.clear();
	// 	Response response;
	// 	Process	process(response, request, _config);
	// 	try
	// 	{
	// 		process.process_request();
	// 	}
	// 	catch (int e)
	// 	{
	// 		process.exception(e);
	// 	}
		
	// 	std::string httpResponse(response.get_response());

	// 	//std::cout << "httpResponse: " << httpResponse << std::endl;
	// 	int bytes_send;
	// 	bytes_send = 0;
	// 	// if bytes_send < len we have to handle it and keep send until everything was sent
	// 	while (bytes_send < (int)httpResponse.length())
	// 	{
	// 		//if (bytes_send == -1)
	// 			// if send return -1, throw error; + perror
	// 		bytes_send = send(forward, httpResponse.data(), httpResponse.length(), 0);
	// 		httpResponse.erase(0, bytes_send);
	// 	}

	// 	close(forward);
	// 	//if close(server) //throw error; + perror
	// }
}
