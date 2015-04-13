/*
 * HttpResponse.cpp
 *
 *  Created on: Apr 12, 2015
 *      Author: yzhh
 */

#include "HttpResponse.hpp"
#include <cstring>

char * HttpResponse::to_rawdata()
{
	strcpy(resp, OK200);
	return resp;
}


