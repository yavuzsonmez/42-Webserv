<h1>Webserv is an HTTP/1.1 server</h1>

<code style="background-color:#3b3939; color:#ffffff">Prototype </code></br>
<code style="background-color:#3b3939; color:#98d6c1">How we use the fnct </code></br>
<p style="color:#2e88f0;">@brief </p>
<p style="color:#cf3e3e;">@note </p>
<p style="color:#3ecf7a;">@param </p>
<p style="color:#c066e3;">@return </p>


---

<code style="background-color:#3b3939; color:#ffffff">int socket(int domain, int type, int protocol);</code> </br>
<code style="background-color:#3b3939; color:#98d6c1">socket(AF_INET, SOCK_STREAM, 0)</code> </br>
<small style="color:#2e88f0;">@brief </small><strong><u>Create a TCP socket</u></strong></br>
<small style="color:#cf3e3e;">@note </small>A stream socket must be in a "connected" state before any data may be sent or received on it.</br>
<small style="color:#3ecf7a;">@param </small><strong>AF_INET</strong>, ipv4 protocol</br>
<small style="color:#3ecf7a;">@param </small><strong>SOCK_STREAM</strong>, TCP as transport layer</br>
<small style="color:#3ecf7a;">@param </small><strong>0</strong>, protocol for SOCK_STREAM which TCP</br>
<small style="color:#c066e3;">@return </small>On success, a file descriptor for the new socket is returned. On error, -1 is returned.</br>

---

<code style="background-color:#3b3939; color:#ffffff">int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);</code> </br>
<code style="background-color:#3b3939; color:#98d6c1">bind(server_fd, (struct sockaddr *)&address, sizeof(address))</code> </br>
<small style="color:#2e88f0;">@brief </small><strong><u>Bind a socket to a Network-Interface:Port</u></strong></br>
<small style="color:#cf3e3e;">@note </small> When a socket is created with socket, it exists in a name space (address family) but has no address assigned to it. bind assigns the address specified by addr to the socket referred to by the file descriptor sockfd.</br>
<small style="color:#3ecf7a;">@param </small><strong>server_fd</strong>, file descriptor returned by socket()</br>
<small style="color:#3ecf7a;">@param </small><strong>address</strong>, <code style="background-color:#3b3939; color:#98d6c1">struct sockaddr_in address;</code> contains network parameters <code style="background-color:#3b3939; color:#98d6c1">address.sin_family = AF_INET;</br>address.sin_addr.s_addr = INADDR_ANY; address.sin_port = htons( PORT );</code></br>
<small style="color:#3ecf7a;">@param </small><strong>sizeof(address)</strong>, len of the network addr param</br>
<small style="color:#c066e3;">@return </small>On success, zero is returned.  On error, -1 is returned.</br>
