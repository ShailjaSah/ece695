/*
 * Socket.hpp
 *
 *  Created on: May 7, 2015
 *      Author: zhihao
 */

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <sys/socket.h>

class Socket {

	int fd;

public:
	Socket() {
		fd = socket(PF_INET, SOCK_STREAM, 0);
	}
	void setNonBlocking();
};

#endif /* SOCKET_HPP_ */
