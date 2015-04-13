/*
 * HttpResponse.hpp
 *
 *  Created on: Apr 12, 2015
 *      Author: yzhh
 */

#ifndef HTTPRESPONSE_HPP_
#define HTTPRESPONSE_HPP_


class HttpResponse {

public:
	char *to_rawdata();

private:
	const char *fakeindex = "<head><head><title>index.html</title></head><body>index.html</body>";
	const char *OK200 = "HTTP/1.1 200 OK\r\n";
	const static int BUF_SIZE = 2048;

	char resp[BUF_SIZE];

};


#endif /* HTTPRESPONSE_HPP_ */
