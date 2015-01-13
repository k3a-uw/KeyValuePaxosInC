/*
 ============================================================================
 Name        : TCSS558.c
 Author      : Kevin Anderson <k3a@uw.edu> & Daniel Kristiyanto <danielkr@uw.edu>
 Version     : 2015.01.10
 Description : TCSS558 Project 1
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.c"
#include "server.c"

int main(int argc, char *argv[]) {
	if (strcmp(argv[0], "client") == 0)
	{
		client_main();
	} else if (strcmp(argv[0], "client") == 0) {
		server_main();
	} else {
		puts("Invalid Run Configuration.");
		return -1;
	}

}
