//============================================================================
// Name        : epll.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <libgen.h>

using namespace std;

int main(int argc, char *argv[]) {
	if (argc <= 2) {
		cout << "usage: " << basename(argv[0]) << " ip port" << endl;
		return 1;
	}

	const char *ip = argv[1];
	int port = atoi(argv[2]);

	int ret = 0;


	return 0;
}
