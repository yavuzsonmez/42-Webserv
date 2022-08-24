<h1>Webserv is an HTTP/1.1 server written in C++ 98.</h1>

<h2>We decided to work with poll for the Input/Output Multiplexing</h2>

<code style="background-color:#3b3939; color:#ffffff">Prototype </code></br>
<code style="background-color:#3b3939; color:#98d6c1">How we use the fnct </code></br>
<p style="color:#2e88f0;">@brief </p>
<p style="color:#cf3e3e;">@note </p>
<p style="color:#3ecf7a;">@param </p>
<p style="color:#c066e3;">@return </p>

---

<a target="_blank" href="#socket">socket</a></br>
<a target="_blank" href="#bind">bind</a></br>
<a target="_blank" href="#listen">listen</a></br>
<a target="_blank" href="#accept">accept</a></br>

<a target="_blank" href="#poll">poll</a></br>


---

<code id="socket" style="background-color:#3b3939; color:#ffffff">int socket(int domain, int type, int protocol);</code> </br>
<code style="background-color:#3b3939; color:#98d6c1">socket(AF_INET, SOCK_STREAM, 0)</code> </br>
<small style="color:#2e88f0;">@brief </small><strong><u>Create a TCP socket</u></strong></br>
<small style="color:#cf3e3e;">@note </small>A stream socket must be in a "connected" state before any data may be sent or received on it. syscall man (2)</br>
<small style="color:#3ecf7a;">@param </small><strong>AF_INET</strong>, ipv4 protocol</br>
<small style="color:#3ecf7a;">@param </small><strong>SOCK_STREAM</strong>, TCP as transport layer</br>
<small style="color:#3ecf7a;">@param </small><strong>0</strong>, protocol for SOCK_STREAM which TCP</br>
<small style="color:#c066e3;">@return </small>On success, a file descriptor for the new socket is returned. On error, -1 is returned, and errno is set to indicate the error.</br>

---

<code id="bind" style="background-color:#3b3939; color:#ffffff">int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);</code> </br>
<code style="background-color:#3b3939; color:#98d6c1">bind(server_fd, (struct sockaddr *)&address, sizeof(address))</code> </br>
<small style="color:#2e88f0;">@brief </small><strong><u>Bind a socket (file-descriptor) to a Network-Interface:Port</u></strong></br>
<small style="color:#cf3e3e;">@note </small> When a socket is created with socket, it exists in a name space (address family) but has no address assigned to it. bind assigns the address specified by addr to the socket referred to by the file descriptor sockfd. syscall man (2)</br>
<small style="color:#3ecf7a;">@param </small><strong>server_fd</strong>, file descriptor returned by socket()</br>
<small style="color:#3ecf7a;">@param </small><strong>address</strong>, <code style="background-color:#3b3939; color:#98d6c1">struct sockaddr_in address;</code> contains network parameters <code style="background-color:#3b3939; color:#98d6c1">address.sin_family = AF_INET;</br>address.sin_addr.s_addr = INADDR_ANY; address.sin_port = htons( PORT );</code></br>
<small style="color:#3ecf7a;">@param </small><strong>sizeof(address)</strong>, len of the network addr param</br>
<small style="color:#c066e3;">@return </small>On success, zero is returned.  On error, -1 is returned, and errno is set to indicate the error.</br>

---

<code id="listen" style="background-color:#3b3939; color:#ffffff">int listen(int sockfd, int backlog);</code> </br>
<code style="background-color:#3b3939; color:#98d6c1">listen(server_fd, 10)</code> </br>
<small style="color:#2e88f0;">@brief </small><strong><u>Listen for connections on a socket</u></strong></br>
<small style="color:#cf3e3e;">@note </small>will be used to accept incoming connection requests using accept. syscall man (2)</br>
<small style="color:#3ecf7a;">@param </small><strong>server_fd</strong> filedescriptor of the socket previously created.</br>
<small style="color:#3ecf7a;">@param </small><strong>server_fd</strong> maximum length to which the queue of pending connections for the provided socket filedescriptor may grow. If full, the client may get an error except if the underlying protocol support retransmission.</br>
<small style="color:#c066e3;">@return </small>On success, zero is returned.  On error, -1 is returned, and errno is set to indicate the error.</br>

---

<code id="accept" style="background-color:#3b3939; color:#ffffff"> int accept(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict addrlen);</code> </br>
<code style="background-color:#3b3939; color:#98d6c1">accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)</code> </br>
<small style="color:#2e88f0;">@brief </small><strong><u>Accept a connection on a socket</u></strong></br>
<small style="color:#cf3e3e;">@note </small>Extracts the first connection request on the queue of pending connections for the listening socket, returns a new file descriptor referring to a new socket. New socket not listening, origin not affected. syscall man (2)</br>
<small style="color:#3ecf7a;">@param </small><strong>server_fd</strong> filedescriptor of the socket previously created.</br>
<small style="color:#3ecf7a;">@param </small><strong>(struct sockaddr *)&address</strong> Pointer to the struct containing network parameters previously created.</br>
<small style="color:#3ecf7a;">@param </small><strong>(socklen_t*)&addrlen)</strong> len of the struct.</br>
<small style="color:#c066e3;">@return </small>On success, return a file descriptor for the accepted socket (a nonnegative integer). On error, -1 is returned, errno is set to indicate the error</br>

---

<code id="poll" style="background-color:#3b3939; color:#ffffff"> int poll(struct pollfd *fds, nfds_t nfds, int timeout);</code> </br>
<small style="color:#2e88f0;">@brief </small><strong><u>Waits for one of a set of file descriptors to become ready to perform I/O.</u></strong></br>

<small style="color:#cf3e3e;">@note </small>Use an array of struct of type pollfd {int fd; short events; short revents; };</br>
<small style="color:#3ecf7a;">@param </small><strong>*fds</strong> Array of struct pollfd.</br>
<small style="color:#3ecf7a;">@param </small><strong>nfds</strong> number of items in the fds array.</br>
<small style="color:#3ecf7a;">@param </small><strong>timeout</strong> len of the struct.</br>
<small style="color:#c066e3;">@return </small>number of milliseconds that poll() should block waiting for a file descriptor to become ready</br>
