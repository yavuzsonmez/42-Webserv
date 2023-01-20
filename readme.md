# webserv


"Mostly Harmless" Webserv project at 42 Heilbronn.

Usage: `./webserv [config_file]`

Built by pweinsto, ysonmez and jfritz.

## How to build

If you make the project and want to see logs, make sure to use the `make debug` or `make rebug` commands.   
By default we do not display any logs from the debugger.

### Requirements

- C++98 (yes, really.)
- macOS or Linux
- Make

## Features

- [x] HTTP/1.1
- [x] GET, POST, HEAD, DELETE
- [x] CGI (PHP, Python, Ruby, whatever you want actually! :D)
- [x] Chunked Transfer
- [x] Autoindex
- [x] Customizable Error pages
- [x] Multiple Server Blocks in one config file
- [x] Multiple Hosts in one Server Block
- [x] POST Body Limit
- [x] Redirects
- [x] Advanced Configuration Parsing
- [x] Advanced Logging
- [x] and much more!

### Config file

The config file is a simple text file that contains the configuration of the server.

The config file is composed of multiple server blocks. Each server block is composed of a server name, a port, a root directory, and a list of locations.

The server name and port are used to identify the server block. The root directory is the directory that will be used as the root directory for the server block. The locations are used to define the behavior of the server for a specific path.
You can define a rough definition of all of the available tokens used by running looking into the PARSING.MD file.

#### CGI

The CGI is a way to execute scripts on the server. The server will execute the script and send the output to the client.
It runs everything from php, to python to ruby. It is a very powerful feature and you probably should not use it in production, like this whole system.

The server will execute the script with basic environment variables. Most stuff should run out of the box, but if you need to add more environment variables, you can do so by adding a `env` directive in the CGI block. It is straight forward.

#### Autoindex

The directory_listing directive is used to enable or disable the autoindex feature. If the autoindex is enabled, the server will list the content of the directory if the client requests a directory. It cannot be used at the same time as the index directive.

#### Error pages

The error page directives are used to define a custom error page for a specific error code. The path must be a valid path to a file. The file must be readable by the server.

#### Multiple Server Blocks

The server block is used to define a server. The server block is composed of a server name, a port, a root directory, and a list of locations. Also, there are cgi_directives that can be defined.

The server name and port are used to identify the server block. The root directory is the directory that will be used as the root directory for the server block. The locations are used to define the behavior of the server for a specific path.

> DISCLAIMER: This project was made for educational purposes only. It is not meant to be used in production. It is not secure, it is not very fast, it is not stable. It was not tested for any vulnerabilites and there might be plenty.