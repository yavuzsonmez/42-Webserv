## webserv

Webserv project at 42.

Usage: `./webserv [config_file]`

Attention:   

If you make the project and want to see logs, make sure to use the `make debug` or `make rebug` commands.   
By default we do not display any logs from the debugger.

TODO:

- Multithreading was a good idea, but introduces a lot of problems. That is why the multiplexer will be put in a seperate class instead of ServerSocket to make it possible to handle multiple server blocks at once without using multithreading. Also, multithreading is not really needed for this project. We will still keep the multithreading branch for future reference, even if it was implemented faulty and in a bad way in case we need to fallback. I will implement a new branch for the multiplexer and reuse some of the code from the multiplexer in ServerSocket. We can pass the whole configuration of the server along to use in the Process.hpp and the Process.hpp and just see what fits to the given Request by the Host-Header. We will have to make some adjustments in the Parsing of the Configuration File but it should be straight forward.

Multiplexer:
	All server ports run now over a single server socket. The Server socket also receives the full configuration file and passes it on to the client socket. The client socket will then the correct server block to each Process.cpp based on hostname/server port.

- To improve performance, cache the serverblocks and do not run the getServerBlocksForPorts/Names again and again for every request.
- As first server block is currently the default server block.


TODO:
- Remove leaks from 
	char **map_to_array(std::map<key, value> &map)
	and
	char **vec_to_array(std::vector<T> &vector)
- It seems like we are not closing all the forks we open. We should look into this.