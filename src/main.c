/*
 * main.c
 *
 *  Created on: Jan 18, 2015
 *      Author: kevanderson
 */

#ifndef MAIN_H
#include "main.h"
#endif


int main(int argc, char * argv[])
{

	char* hostname[50];
	unsigned short port_num;
	validateCommandLine(argc, argv, hostname, &port_num);

	char user_input[128];  //allow for extra chars to be typed, but ignore them
	while(1)
	{
		printMenu(argc);
		fgets(user_input, 128, stdin); //allow for extra chars to be typed

		switch (user_input[0]) {
			case '1':
				if (argc > 2)
					client_udp_init(hostname, port_num);
				else
					server_udp_init(port_num);
				break;
			case '2':
				if (argc > 2)
					client_tcp_init(hostname, port_num);
				else
					server_tcp_init(port_num);
				break;
			case 'Q':
			case 'q':
				printf("Goodbye.");
				exit(0);
			default:
				printf("Invalid Entry.  Try again\n");
				break;
		}
	}
}

int validateCommandLine(int argc, char * argv[], char* hostname, unsigned short* port_num)
{
	// CHECK THE NUMBER OF ARGUMENTS.  IF ARG = 2 THEN PORT IS ON INDEX 1, OTHERWISE IT IS ON INDEX 2
	int i;
	if (argc <= 1) //NOT ENOUGH
	{
		printf("Usage: (for Server) main port  |  (for client) main server_ip_or_hostname port.\n");
		exit(-1);
	} else if (argc == 2) {  // CLIENT STUFF
		i = 1;
		sprintf(hostname, "N/A");
	} else {
		i = 2;
		sprintf(hostname, argv[1]);
	}

	//  GET THE PORT NUMBER
	int port_result = validatePort(argv[i]);
	if (port_result > 0) {
		*port_num = (unsigned short) port_result;
	} else {
		printf("The port is invalid.  Please provide a port between 1000 and 65534 (inclusive).");
		exit(-1);
	}
}

int validatePort(char* arg) {
	long port_l;
	unsigned short port;
	char *toss;
	port_l = strtol(arg, &toss, 10);

	if (port_l < 65535 && port_l >= 1000)  // we have a good port
	{
		return (int) port_l;  //return a valid port number
	} else {
		return(-1);
	}

}

void printMenu(int argc)
{
	if (argc > 2)
	{
		printf("Based on the number of arguments [%d] you will launch the CLIENT environment.\n",argc);
	} else if (argc == 2) {
		printf("Based on the number of arguments [%d] you will launch the SERVER environment.\n", argc);
	} else {
		printf("Usage: (for Server) main port  |  (for client) main server_ip_or_hostname port.\n");
		exit(-1);
	}

	printf("Please Choose a protocol:\n  1. UDP\n  2. TCP\n  Q. Quit \n>> ");
}
