#include "ServerSocket.h"

net::ServerSocket::ServerSocket(std::string addr, uint16_t port, unsigned int backlog)
	: addr(addr), port(port), backlog(backlog) {
}

net::ServerSocket::~ServerSocket() {
	close();
}

const int net::ServerSocket::listen() {
	struct sockaddr_in addr_in = net::Socket::get_struct(addr, port);
	/* system call: create a socket
	 * AF_INET = IPv4 Internet protocols
	 * SOCK_STREAM = sequenced, reliable, two-way, connection-based byte streams
	 * IPPROTO_TCP = protocol to be used */
	socketfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketfd == -1) {
		return errno;
	}
	/* system call: set some options for the socket
	 *  SOL_SOCKET = manipulate options at the sockets API level
	 *  SO_REUSEADDR = reuse same address in some corner cases of short connection loss */
	int yes = 1;
	if (::setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) != 0) {
		close();
		return errno;
	}
	// system call: bind the previously created socket to an address
	if (::bind(socketfd, (struct sockaddr*)&addr_in, sizeof(struct sockaddr)) != 0) {
		close();
		return errno;
	}
	/* system call: marks the socket referred to by sockfd as a passive socket,
     * that is, as a socket that will be used to accept incoming connection requests */
	if (::listen(socketfd, backlog) != 0) {
		close();
		return errno;
	}
	return 0;
}

net::Socket* net::ServerSocket::accept() const {
	struct sockaddr_in addr_in;
	socklen_t len = sizeof(addr_in);
	int clientfd = ::accept(socketfd, (struct sockaddr*)&addr_in, &len);
	return new net::Socket(clientfd, addr_in);
}

const bool net::ServerSocket::valid(){
	return socketfd != -1;
}

void net::ServerSocket::close() const{
	if (socketfd == -1){
		return;
	}
	::close(socketfd);
}