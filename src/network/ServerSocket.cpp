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
					_clients.push_back(std::pair<int, ClientSocket>(forward, ClientSocket(clientSocket, _config, forward)));
				}
			}
			else
			{
				client_iter	pos;
				if (pollfds[i].revents == POLLIN)
				{
					try {
						pos = get_CS_position(_clients, pollfds[i].fd);
						(*pos).second.call_func_ptr();
						if ((*pos).second._remove)
						{
							_clients.erase(pos);
							std::vector<pollfd>::iterator	del = pollfds.begin() + i;
							pollfds.erase(del);
						}
						else
						{
							pollfds[i].events = (*pos).second._event;
							pollfds[i].fd = (*pos).second._fd;
							(*pos).first = (*pos).second._fd;
						}
					}
					catch (std::string error) {

						if (error == Request_Timeout)
							pollfds[i].events = POLLOUT;
					}
				}
			 	else if (pollfds[i].revents == 	POLLOUT)
				{
					pos = get_CS_position(_clients, pollfds[i].fd);
					(*pos).second.call_func_ptr();
					if ((*pos).second._remove)
					{
						_clients.erase(pos);
						std::vector<pollfd>::iterator	del = pollfds.begin() + i;
						pollfds.erase(del);
					}
					else
					{
						pollfds[i].events = (*pos).second._event;
						pollfds[i].fd = (*pos).second._fd;
						(*pos).first = (*pos).second._fd;
					}
				}
			}
		}
	}
}

client_iter	ServerSocket::get_CS_position(std::vector<std::pair<int, ClientSocket> > &vector, int key)
{
	client_iter	it;
	client_iter ite	= vector.end();
	for (it = vector.begin(); it < ite; it++)
	{
		if ((*it).first == key)
			return it;
	}
	return ite;
}


