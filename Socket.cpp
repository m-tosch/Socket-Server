#include "Socket.h"

#define DEFAULT_SOCKET_BUFFER 256

net::Socket::Socket(int socketfd, struct sockaddr_in addr_in) 
	: socketfd(socketfd), addr_in(addr_in) {
}

net::Socket::~Socket() {
	close();
}

const int net::Socket::read(std::string& msg) {
	int bytes_total = 0;
	char buf[DEFAULT_SOCKET_BUFFER];

	// blocking call until data is present
	int bytes_read = recv(socketfd, buf, DEFAULT_SOCKET_BUFFER, 0);
	if (bytes_read <= 0) {
		return bytes_read;
	}
	msg.append(std::string(buf, 0, bytes_read));
	bytes_total += bytes_read;

	// set non-blocking
	set_unblocking();
	while (bytes_read > 0) {
		memset(buf, 0, DEFAULT_SOCKET_BUFFER);
		// system call recv is non blocking here so it will not stall
		bytes_read = recv(socketfd, buf, DEFAULT_SOCKET_BUFFER, 0);
		if (bytes_read < 0) {
			break;
		}
		msg.append(std::string(buf, 0, bytes_read));
		bytes_total += bytes_read;
	}
	// set back to blocking
	set_blocking();
	return bytes_total;
}

const int net::Socket::read(char* buf, size_t len) {
	// this recv system call will stall
	return recv(socketfd, buf, len, 0);
}

const int net::Socket::send(std::string data) const {
	return send(data.c_str(), data.length(), 0);
}

const int net::Socket::send(const char* buf, int len, int flags) const {
	return ::send(socketfd, buf, len, flags);
}

const bool net::Socket::valid() {
    return socketfd != -1;
}

struct sockaddr_in net::Socket::get_struct(const std::string addr, const uint16_t port){
	struct sockaddr_in addr_in_;
	memset(&addr_in_, 0, sizeof addr_in_);
	// AF_INET for IPv4
	addr_in_.sin_family = AF_INET;
	// converts from host byte order to network byte order
	addr_in_.sin_port = htons(port);
	// converts the host address from the IPv4 numbers-and-dots notation
	// into binary form in network byte order
	inet_aton(addr.c_str(), &addr_in_.sin_addr);
	return addr_in_;
}

void net::Socket::set_blocking() {
	// Get the file status flags and file access modes of the socket file descriptor
	int opts = fcntl(socketfd, F_GETFL);
	// clear the non blocking flag (bit mask)
	opts = opts & (~O_NONBLOCK);
	// set file status flag to blocking
	fcntl(socketfd, F_SETFL, opts);
}

void net::Socket::set_unblocking() {
	// set file status flag to non blocking
	fcntl(socketfd, F_SETFL, O_NONBLOCK);
}

void net::Socket::close() const {
	if (socketfd == -1) {
		return;
	}
	::close(socketfd);
}