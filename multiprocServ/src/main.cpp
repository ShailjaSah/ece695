/*
 * main.cpp
 *
 *  Created on: Apr 9, 2015
 *      Author: yzhh
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>


#include <libgen.h>

#include <iostream>
#include <cassert>

#include "HttpHandler.hpp"

using namespace std;


void handle_child(int sig)
{
	pid_t pid;
	int stat;
	while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
		cout << pid << "end" << endl;
	}
}


int main(int argc, char **argv)
{
	if (argc != 2) {
		cout << "usage: " << basename(argv[0]) << " port" << endl;
		exit(1);
	}

	signal(SIGCHLD, handle_child);

	int port = atoi(argv[1]);
	int server_sock, client_fd;
	struct sockaddr_in client_addr, serv_addr;

	server_sock = socket(PF_INET, SOCK_STREAM, 0);
	assert(server_sock >= 0);

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);

	int ret = bind(server_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	assert(ret != -1);

	ret = listen(server_sock, 5);
	assert(ret != -1);

	socklen_t cli_addrlen = sizeof(client_addr);
	pid_t child;

	while (1){
		client_fd = accept(server_sock, (struct sockaddr *)&client_addr, &cli_addrlen);

		if (client_fd < 0) {
			cout << "accept error : " << strerror(errno) << endl;
		} else {
			child = fork();
			// child process
			if (child == 0) {
				close(server_sock);
				cout << "new client from :" << inet_ntoa(client_addr.sin_addr) << endl;
				HttpHandler httpHandler(client_fd);
				httpHandler.run();
				exit(0);
			} else if (child > 0) {
				cout << "new child: " << child << endl;
				close(client_fd);
			} else {
				cout << "fork error: " << strerror(errno) << endl;
			}
		}
	}
	exit(0);
}


