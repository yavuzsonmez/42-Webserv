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
 * @brief Removes the socket from the vector and closes it.
 * This should be called if the client was not yet created
 * @param pollfds the pollfds 
 * @param i index
 */
void ServerSocket::socketFailed(std::vector<pollfd> &pollfds, int i)
{
	close(pollfds[i].fd);
	pollfds.erase(pollfds.begin() + i);
	std::cout << "Socket closed because it failed." << std::endl;
}

/**
 * @brief Disconnects the given client from the server
 * 
 * @param pollfds the pollfds 
 * @param i index
 */
void ServerSocket::disconnectClient(std::vector<pollfd> &pollfds, int i)
{
	client_iter pos = get_CS_position(_clients, pollfds[i].fd);
	close(pollfds[i].fd);
	pollfds.erase(pollfds.begin() + i);
	if (pos != _clients.end())
		_clients.erase(pos);
	std::cout << "Client removed and connection closed." << std::endl;
}

/**
 * @brief Here we check if the client connection broke. If it does, we make sure to remove it.
 * A connection to a client is being considered broken if the client has an revent of
 * POLLERR, POLLHUP.
 * @param pollfds poll fds
 * @param i index
 */
void ServerSocket::checkIfConnectionIsBroken(std::vector<pollfd> &pollfds, int i)
{
	if (pollfds[i].revents & ((POLLERR | POLLHUP)))
	{
		std::cout << "Connection broken. Error code: " << pollfds[i].revents << std::endl;
		disconnectClient(pollfds, i);
		return ;
	}
	if (pollfds[i].revents & POLLNVAL)
	{
		std::cout << "Found an invalid connection. Taking care. " << pollfds[i].revents << std::endl;
		client_iter pos = get_CS_position(_clients, pollfds[i].fd);
		pollfds.erase(pollfds.begin() + i);
		if (pos != _clients.end())
			_clients.erase(pos);
		return ;
	}
}

/**
 * @brief Here we listen on the ports and accept new incoming connections.
 * 
 * @param pollfds the pollfd vector
 * @param i index of the current pollfd
 * 
 * @returns false, if a client was declined
 */
bool ServerSocket::acceptNewConnectionsIfAvailable(std::vector<pollfd> &pollfds, int i) {
	USE_DEBUGGER;

	struct pollfd tmp;
	struct sockaddr_in clientSocket;
	socklen_t socketSize = sizeof(clientSocket);

	int forward;
	forward = accept(pollfds[i].fd, (struct sockaddr *)&clientSocket, &socketSize);
	std::cout << "New connection accepted on fd " << forward << std::endl;
	if (forward == -1) return false;
	tmp.fd = forward; // set the newly obtained file descriptor to the pollfd. Important to do this before the fcntl call!
	int val = fcntl(forward, F_SETFL, fcntl(forward, F_GETFL, 0) | O_NONBLOCK);
	if (val == -1) { // fcntl failed, we now need to close the socket
		debugger.verbose("fcntl failed. Closing socket.");
		close(forward);
		return false; 
	};
	if (_clients.size() >= MAXIMUM_CONNECTED_CLIENTS) {
		debugger.debug("Maximum number of clients reached. Declining connection.");
		int result = send_server_unavailable(forward, _config);
		if (result == -1) {
			debugger.verbose("Error while sending 503 to client. Closing connection.");
			close(forward);
			return false;
		}
		debugger.verbose("Closing connection 3");
		close(forward);
		return false;
	}

	// set the pollfd to listen for POLLIN events (read events)
	tmp.events = POLLIN;
	tmp.revents = 0;
	pollfds.push_back(tmp); //add the new fd/socket to the set, considered as "client"
	_clients.push_back(std::pair<int, ClientSocket>(forward, ClientSocket(clientSocket, _config, forward))); //Link the forwarded fd to a new client
	return true;
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

	//setup the expected event for the listening sockets to "read"
	for (unsigned int i = 0; it != ite; ++it, ++i)
	{
		pollfds[i].fd = *it;
		pollfds[i].events = POLLIN;
		pollfds[i].revents = 0;
	}
	
	// TODO:
	// Remove the unreadable function pointers.
	// See, when the response is actually being sent
	// Main routine. This will be called the whole time the server runs
	while (1) {
		if (poll((struct pollfd *)(pollfds.data()), pollfds.size(), -1) < 1) // Here we wait for poll information.
			std::cout << "An error occured when polling.";
		for (unsigned long i = 0; i < pollfds.size(); ++i) //iterate through the entire area of sockets
		{
			/**
			 * Listen to the listening sockets for new connections (ports)
			 */
			if (i < listeningSockets)
			{
				if (pollfds[i].revents == POLLIN)
					if (!acceptNewConnectionsIfAvailable(pollfds, i))
						continue;
			}
			/**
			 * Listen to established connections
			 */
			else //area of ClientSocket, sockets that are the result of a forwarded fd (accepted connection), and that we consider as client.
			{
				client_iter	pos;
				pos = get_CS_position(_clients, pollfds[i].fd); //retrieve the right client
				if (pollfds[i].revents == POLLIN) //Client is ready for reading, so we try to read the entire request.
				{
					(*pos).second.call_func_ptr(); //execute the next operation on the fd
					if ((*pos).second.Timeout()) //if the client timeouts, remove it from the list.
					{
						debugger.verbose("Client timed out.");
						disconnectClient(pollfds, i);
						continue;
					}
					if ((*pos).second._remove) // If a client asks to be removed, remove it from the list
					{
						debugger.verbose("Client asked to be removed.");
						disconnectClient(pollfds, i);
						continue;
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
					if ((*pos).second._remove) // The client asks to be removed
					{
						disconnectClient(pollfds, i);
						continue;
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
					checkIfConnectionIsBroken(pollfds, i);
					continue;
				}
			}
		}
	}
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
