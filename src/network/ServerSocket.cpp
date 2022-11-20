#include "../../inc/network/ServerSocket.hpp"
#include "../../inc/http/Response.hpp"
#include "../../inc/http/Process.hpp"
#include "../../inc/debugger/DebuggerPrinter.hpp"
#include <sys/ioctl.h>


/**
 * @brief Setup, bind and put the sockets in listening mode. (Ports of one server Block)
 * @param config, parsed server config file.
 * @param address, network interface where to listen.
 */
ServerSocket::ServerSocket(ServerBlock config, unsigned int address) : _config(config)
{
	std::vector<struct sockaddr_in>::iterator	so;
	listeningSockets = _config.getAllServerPorts().size();
	_sockets.resize(listeningSockets);
	int	i = 0;
	for (so = _sockets.begin(); so != _sockets.end(); so++, i++) //configure the sockets
	{
		(*so).sin_family = AF_INET;
		(*so).sin_port = htons(_config.getAllServerPorts()[i]);
		(*so).sin_addr.s_addr = address;
		bzero(&((*so).sin_zero), 8);
	}
	const int	enable = 1;
	_fds.resize(_sockets.size());
	std::vector<int>::iterator	fd;
	for (fd = _fds.begin(); fd != _fds.end(); fd++) //setup the sockets and make the fds non blocking
	{
		(*fd) = socket(AF_INET, SOCK_STREAM, 0); //IPv4, TCP
		if (*fd < 0)
			throw SocketCreationError();
		fcntl(*fd, F_SETFL, fcntl(*fd, F_GETFL, 0) | O_NONBLOCK);
		setsockopt(*fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
	}

	for (fd = _fds.begin(), so = _sockets.begin(); fd != _fds.end(); fd++, so++) //bind the fds to the sockets and put them in listening mode
	{
		if (bind(*fd, (struct sockaddr *)&(*so), sizeof(struct sockaddr_in)))
			throw SocketCreationError();
		if (listen(*fd, BACKLOG)) //backlog is the length of the queue for the upcoming connections
			throw SocketCreationError();
	}
	processConnections();
}

ServerSocket::~ServerSocket(){}

/**
 * @brief removes a client if necessary
 * 
 * @param client 
 * @param position of current client in the vector
 */
void ServerSocket::removeIfNecessary(std::vector<pollfd> pollfds, int i)
{
	if (pollfds[i].revents & POLLHUP || pollfds[i].revents & POLLERR || pollfds[i].revents & POLLNVAL)
	{
		close(pollfds[i].fd);
		pollfds.erase(pollfds.begin() + i);
		std::cout << "Client removed" << std::endl;
	}
}

/**
 * @brief Disconnects the given client from the server
 * 
 * @param pollfds the pollfds 
 * @param i index
 * @param client_iter position of the client to be disconnected
 */
void ServerSocket::disconnectClient(std::vector<pollfd> &pollfds, int i, client_iter pos)
{
	close(pollfds[i].fd);
	pollfds.erase(pollfds.begin() + i);
	_clients.erase(pos);
	std::cout << "Client removed and connection closed." << std::endl;
}

/**
 * @brief Here we check if the client connection broke. If it does, we make sure to remove it.
 * A connection to a client is being considered broken if the client has an revent of
 * POLLERR, POLLHUP, POLLNVAL or POLLHUP.
 * @param pollfds poll fds
 * @param i index
 * @param pos position of client
 */
void ServerSocket::checkIfConnectionIsBroken(std::vector<pollfd> &pollfds, int i, client_iter pos)
{
	if (pollfds[i].revents & ((POLLERR | POLLHUP | POLLNVAL | POLLHUP)))
	{
		close(pollfds[i].fd);
		std::vector<pollfd>::iterator	del = pollfds.begin() + i;
		pollfds.erase(del);
		std::cout << "Client removed" << std::endl;
	}
}

/**
 * @brief Here we listen on the ports and accept new incoming connections.
 * 
 * @param pollfds the pollfd vector
 * @param i index of the current pollfd
 */
void ServerSocket::acceptNewConnectionsIfAvailable(std::vector<pollfd> &pollfds, int i) {
	USE_DEBUGGER;
	struct pollfd tmp;
	struct sockaddr_in clientSocket;
	socklen_t socketSize = sizeof(clientSocket);

	int forward;
	forward = accept(pollfds[i].fd, (struct sockaddr *)&clientSocket, &socketSize);
	if (forward == -1) return;
	tmp.fd = forward; // set the newly obtained file descriptor to the pollfd
	int val = fcntl(forward, F_SETFL, fcntl(forward, F_GETFL, 0) | O_NONBLOCK);
	if (val == -1) { // fcntl failed, we now need to close the socket
		disconnectClient(pollfds, i, _clients.begin());
		return;
	};
	tmp.events = POLLIN;
	tmp.revents = 0;
	pollfds.push_back(tmp); //add the new fd/socket to the set, considered as "client"
	_clients.push_back(std::pair<int, ClientSocket>(forward, ClientSocket(clientSocket, _config, forward))); //Link the forwarded fd to a new client
}


/**
 * @brief Handles connections by using POLL
 * and checking if the fds are rdy for I/O operations
 */
void ServerSocket::processConnections()
{
	USE_DEBUGGER;
	std::vector<pollfd> pollfds;
	pollfds.resize(_config.getAllServerPorts().size()); //create a vector of pollfds struct, same length as the number of listening sockets
	std::vector<int>::iterator	it = _fds.begin();
	std::vector<int>::iterator	ite = _fds.end();

	for (unsigned int i = 0; it != ite; ++it, ++i) //setup the expected event for the listening sockets to "read"
	{
		pollfds[i].fd = *it;
		pollfds[i].events = POLLIN;
		pollfds[i].revents = 0;
	}
	// Main routine. This will be called the whole time the server runs
	while (1) {
		if (poll((struct pollfd *)(pollfds.data()), pollfds.size(), -1) < 1) // Here we wait for poll information.
			std::cout << "An error occured when polling."; 
		for (unsigned long i = 0; i < pollfds.size(); ++i) //iterate through the entire set of sockets
		{
			/**
			 * Listen to the listening sockets for new connections (ports)
			 */
			if (i < listeningSockets)
			{
				if (pollfds[i].revents == POLLIN)
					acceptNewConnectionsIfAvailable(pollfds, i);
			}
			/**
			 * Listen to established connections
			 */
			else //Set of ClientSocket, sockets that are the result of a forwarded fd (accepted connection), and that we consider as client.
			{
				client_iter	pos;
				pos = get_CS_position(_clients, pollfds[i].fd); //retrieve the right client
				if (pollfds[i].revents == POLLIN) //Client is ready for reading, so we try to read the entire request.
				{
					(*pos).second.call_func_ptr(); //execute the next operation on the fd
					if ((*pos).second._remove)
					{
						disconnectClient(pollfds, i, pos);
					}
					else
					{
						pollfds[i].events = (*pos).second._event;
						pollfds[i].fd = (*pos).second._fd;
						(*pos).first = (*pos).second._fd;
					}
				}
			 	else if (pollfds[i].revents == 	POLLOUT)
				{
					(*pos).second.call_func_ptr(); //execute the next operation on the fd
					if ((*pos).second._remove) //remove the client
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
				else
				{
					checkIfConnectionIsBroken(pollfds, i, pos);
				}
			}
		}
	}

	//for (unsigned int i = 0; it != ite; ++it, ++i) //setup the expected event for the listening sockets to "read"
	//{
	//	pollfds[i].fd = *it;
	//	pollfds[i].events = POLLIN;
	//	pollfds[i].revents = 0;
	//}
	//while (1) //main loop where the magic happends
	//{
	//	std::cout << "Currently open fds: " << pollfds.size() << std::endl;
	//	//std::cout << "Waiting for connections..." << std::endl;
	//	if (poll((struct pollfd *)(pollfds.data()), pollfds.size(), -1) < 1) //Poll will check if fds are rdy for I/O and fill the revents
	//		std::cout << "An error occured when polling."; 
	//	for (unsigned long i = 0; i < pollfds.size(); ++i) //iterate through the entire set of sockets
	//	{
	//		if (i < listeningSockets) //the first set of struct are the ones of the listening sockets (ServerSockets).
	//		{
	//			removeIfNecessary(pollfds, i); // remove the client if necessary
	//			/**
	//			 * Listening on ports and new connections only
	//			 */
	//			if (pollfds[i].revents == POLLIN) //if they are rdy for "reading" we accept connection and got a new fd that we add to the set.
	//			{
	//				struct pollfd tmp;
	//				forward = accept(pollfds[i].fd, (struct sockaddr *)&clientSocket, &socketSize);
	//				if (forward == -1) continue;
	//				int val = fcntl(forward, F_SETFL, fcntl(forward, F_GETFL, 0) | O_NONBLOCK);
	//				if (val == -1) {
	//					close(forward);
	//					debugger.warning("Closed connection!");
	//					continue;
	//				};
	//				tmp.fd = forward;
	//				tmp.events = POLLIN;
	//				tmp.revents = 0;
	//				pollfds.push_back(tmp); //add the new fd/socket to the set, considered as "client"
	//				_clients.push_back(std::pair<int, ClientSocket>(forward, ClientSocket(clientSocket, _config, forward))); //Link the forwarded fd to a new client
	//			}
	//		}
	//		/**
	//		 * 
	//		 */
	//		else //Set of ClientSocket, sockets that are the result of a forwarded fd (accepted connection), and that we consider as client.
	//		{
	//			client_iter	pos;
	//			if (pollfds[i].revents == POLLIN) //Client is ready for reading, so we try to read the entire request.
	//			{
	//				pos = get_CS_position(_clients, pollfds[i].fd); //retrieve the right client
	//				(*pos).second.call_func_ptr(); //execute the next operation on the fd
	//				if ((*pos).second._remove)
	//				{
	//					_clients.erase(pos);
	//					std::vector<pollfd>::iterator	del = pollfds.begin() + i;
	//					pollfds.erase(del);
	//				}
	//				else
	//				{
	//					pollfds[i].events = (*pos).second._event;
	//					pollfds[i].fd = (*pos).second._fd;
	//					(*pos).first = (*pos).second._fd;
	//				}
	//			}
	//		 	else if (pollfds[i].revents == 	POLLOUT)
	//			{
	//				pos = get_CS_position(_clients, pollfds[i].fd); //retrieve the client associated to the fd
	//				(*pos).second.call_func_ptr(); //execute the next operation on the fd
	//				if ((*pos).second._remove) //remove the client
	//				{
	//					_clients.erase(pos);
	//					std::vector<pollfd>::iterator	del = pollfds.begin() + i;
	//					pollfds.erase(del);
	//				}
	//				else
	//				{
	//					pollfds[i].events = (*pos).second._event;
	//					pollfds[i].fd = (*pos).second._fd;
	//					(*pos).first = (*pos).second._fd;
	//				}
	//			}
	//			else
	//			{
	//				if (pollfds[i].revents & POLLHUP || pollfds[i].revents & POLLERR || pollfds[i].revents & POLLNVAL)
	//				{
	//					close(pollfds[i].fd);
	//					std::vector<pollfd>::iterator	del = pollfds.begin() + i;
	//					pollfds.erase(del);
	//					std::cout << "Client removed" << std::endl;
	//				}
	//			}
	//		}
	//	}
	//}
}

/**
 * @brief Returns the right pair of fd/client corresponding to a fd
 */
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
