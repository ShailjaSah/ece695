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
#include <cassert>


class Server {
private:
	char *ip;
	int port;

	const int MAX_EPOLL_EVENT = 1024;
	epoll_event events[MAX_EPOLL_EVENT];
	int epollfd;

	void epoll_init() {
		epollfd = epoll_create(5);
		assert( epollfd != -1);
	}


public:
	Server(char *ip, char *port) {
		this->ip = ip;
		this->port = atoi(port);
	}

	void listen() {

	}

	void run() {
		int ret = 0;
		while (1) {
			//ret = epoll_wait
		}
	}
};


#endif /* SERVER_HPP_ */
