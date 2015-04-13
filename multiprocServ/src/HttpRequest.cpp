/*
 * HttpRequest.cpp
 *
 *  Created on: Apr 12, 2015
 *      Author: yzhh
 */
#include "HttpRequest.hpp"
#include <string>

using namespace std;

HttpRequest::HttpRequest(char *buf, int size)
{
	//char *token = nullptr;
	const char *deli = "\r\n";
	const string get_str("GET");

	string buf_str(buf);

	size_t pos = buf_str.find(get_str);
	if (pos != string::npos) {
		method = GET;
	}
}

ReqMethod HttpRequest::getMethod()
{
	return this->method;
}


