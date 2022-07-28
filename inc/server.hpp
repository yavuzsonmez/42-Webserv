#ifndef SERVER_HPP
# define SERVER_HPP

class server
{
	public:
	server();
	~server()

	private:
	tcp_socket	_tcp_sockets;
	std::string	_server_name;
	std::string	_access_log;
	std::vector<location>	locations;
};	

# endif