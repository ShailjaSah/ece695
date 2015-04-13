/*
 * HttpRequest.hpp
 *
 *  Created on: Apr 9, 2015
 *      Author: yzhh
 */

#ifndef HTTPREQUEST_HPP_
#define HTTPREQUEST_HPP_

#include <string>

enum ReqMethod {
	GET,
	POST,
	OTHER
};

enum ReqStatus {
	GOOD,
	BAD,
};

class HttpRequest {

public:
	HttpRequest(char *buf, int size);
	ReqMethod getMethod();

private:
	ReqMethod method = OTHER;
	ReqStatus status;
	 //parseRequest
};



#endif /* HTTPREQUEST_HPP_ */
