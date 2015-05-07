/*
 * Server.hpp
 *
 *  Created on: May 7, 2015
 *      Author: zhihao
 */

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <stdlib.h>
#include <sys/epoll.h>


class Server {
private:
	char *ip;
	int port;

	epoll

	const int MAX_EPOLL_EVENT = 1024;

	void epoll_init() {

	}


public:
	Server(char *ip, char *port) {
		this->ip = ip;
		this->port = atoi(port);
	}

	void listen() {

	}

	void run() {

	}
};


#endif /* SERVER_HPP_ */
