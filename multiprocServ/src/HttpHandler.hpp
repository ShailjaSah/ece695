/*
 * HttpHandler.hpp
 *
 *  Created on: Apr 12, 2015
 *      Author: yzhh
 */

#ifndef HTTPHANDLER_HPP_
#define HTTPHANDLER_HPP_



class HttpHandler {

public:
	HttpHandler(int sock);

	void run();

private:
	const static int BUF_SIZE = 2048;
	char buf[BUF_SIZE];
	int data_read;
	int cli_sock;
};


#endif /* HTTPHANDLER_HPP_ */
