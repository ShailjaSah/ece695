/*
 * HttpResponse.cpp
 *
 *  Created on: Apr 12, 2015
 *      Author: yzhh
 */

#include "HttpResponse.hpp"
#include <cstring>
#include <sys/socket.h>
#include <iostream>

using namespace std;

char * HttpResponse::to_rawdata()
{
	strcpy(resp, "OK 200");
	cout << "Response copied to string " << endl;
	return resp;
}


