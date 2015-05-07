/*
 * Socket.hpp
 *
 *  Created on: May 7, 2015
 *      Author: zhihao
 */

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

class Socket {

	struct sockaddr_in address;

public:
	void setNonBlocking();
};

#endif /* SOCKET_HPP_ */
