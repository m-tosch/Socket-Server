#pragma once
#include "Socket.h"

/**
    A network server socket 
	Clients can connect to this server via address/IP & port
	
    @author mtosch
    @version 1.0 22/06/18
*/
namespace net {
	class ServerSocket {
	public:
		/**
		* Constructs a listening socket on the specified port and address with a backlog
		* The backlog specifies how many simultaneous incoming connections are allowed
		* @param the address to bind to
		* @param the port to listen to
		* @param the number of alowed backlogs
		*/
		ServerSocket(std::string, uint16_t, unsigned int = 1);

		/**
		 * Closes the listening socket if necessary
		 */
		~ServerSocket();

		/**
		* Creates the listening socket (file descriptor) and binds it to the current
		* port and address. This socket will then be marked as a passive listening socket
		* @return error code if there was an error
		*/
		const int listen();

		/**
		* Accepts a new incomming client
		* A call to this function is blocking!
		* @return A new client socket instance
		*/
		net::Socket *accept() const;

		/**
		* Checks whether or not the socket is valid
		* @return true the socket is valid, false otherwise
		*/
		const bool valid();

	  private:
	  	// IP
		std::string addr;
		// port, 16-bit [0:65535]
		uint16_t port;
		// backlog queue for incoming connections
		unsigned int backlog;
		// socket file descriptor
		int socketfd; 

		/**
		* Closes the socket if it exists
		*/
		void close() const;
	};
};
