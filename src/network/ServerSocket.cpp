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

/**
 * @brief Handles connections 
 */
void ServerSocket::processConnections()
{
	std::map<int, unsigned long>	router;
	
	int forward;
	struct sockaddr_in clientSocket;
	socklen_t socketSize = sizeof(struct sockaddr_in);
	
	std::vector<pollfd> pollfds;
	pollfds.resize(_config.getAllServerPorts().size());

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
		if (poll((struct pollfd *)(pollfds.data()), pollfds.size(), -1) < 1)
			std::cout << "An error occured when polling.";
		for (unsigned long i = 0; i < pollfds.size(); ++i)
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
					router.insert(std::pair<int, unsigned long>(forward, _clients.size()+1));
					_clients.insert(std::pair<unsigned long, ClientSocket>(_clients.size()+1, ClientSocket(clientSocket, _config, forward)));
					//_clients.insert(std::pair<int, ClientSocket>(forward, ClientSocket(clientSocket, _config, forward)));
				}
			}
			else
			{
				if (pollfds[i].revents == POLLIN)
				{
					try {
						if (!(_clients.at(router.at(pollfds[i].fd)).read_in_buffer()))
						{
							pollfds[i].events = POLLOUT;
						}
					}
					// try {
					// 	if (!(_clients.at(pollfds[i].fd).read_in_buffer()))
					// 	{
					// 		pollfds[i].events = POLLOUT;
					// 	}
					// }
					catch (std::string error) {

						if (error == Request_Timeout)
							pollfds[i].events = POLLOUT;
					}
				}
			 	else if (pollfds[i].revents == 	POLLOUT)
				{
					if (!_clients.at(router.at(pollfds[i].fd)).write_from_buffer())
					{
						_clients.erase(router.at(pollfds[i].fd));
						router.erase(pollfds[i].fd);
						std::vector<pollfd>::iterator	del = pollfds.begin() + i;
						pollfds.erase(del);
					}
				}
			}
		}
	}
}
