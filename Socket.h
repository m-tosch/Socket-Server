#pragma once
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <memory>

/**
    A basic network socket

    @author mtosch
    @version 1.0 22/06/18
*/
namespace net{
	class Socket{
	public:
	    // socket file descriptor
		int socketfd;
		// system socket struct that encapsulates address & port
		struct sockaddr_in addr_in;

		/**
		* Constructs a new socket instance given a file descriptor and an address
		* @param the socket file descriptor
		* @param the address encapsulated in a sockaddr_in struct
		*/
		Socket(int, struct sockaddr_in);

		/**
		 * Closes the listening socket if necessary
		 */
		~Socket();

		/**
		 * Reads all data being sent from the client by writing it to the specified
		 * string objects address. The function will block until there's data, and 
		 * then read the rest if any
		 * @param the string object to save the data into
		 * @return total bytes received
		 */
		const int read(std::string &);

		/**
		 * Reads a specified amount of data into a character pointer
		 * @param the character buffer
		 * @param the length of the character buffer
		 * @return error code if there was an error
		 */
		const int read(char*, size_t);

		/**
		 * Sends a string to the client
		 * @param the string to send
		 * @return error code if there was an error
		 */
		const int send(std::string) const;

		/**
		 * Sends an array of charactes with a fixed length to the client
		 * @param the character buffer
		 * @param the length
		 * @param the flags
		 * @return error code if there was an error
		 */
		const int send(const char*, int, int = 0) const;

		/**
		 * Checks whether the socket (file descriptor) is valid
		 * @return true if the socket is valid, false otherwise
		 */
		const bool valid();

		/**
		* Returns a sockaddr_in structure based on a raw address (ip) and port
		* @param the address (ip)
		* @param the port
		* @return the created sockaddr_in struct
		*/
		static struct sockaddr_in get_struct(const std::string, const uint16_t);

	private:
		/**
		 * Sets the socket in blocking mode
		 */
		void set_blocking();

		/**
		 * Sets the socket in non-blocking mode
		 */
		void set_unblocking();

		/**
		 * Closes the socket if it exists. This method is called on object destruction
		 */
		void close() const;
	};
};