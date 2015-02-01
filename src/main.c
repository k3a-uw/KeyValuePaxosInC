/*
 ============================================================================
 Name        : main.c
 Author      : Kevin Anderson <k3a@uw.edu> & Daniel Kristiyanto <danielkr@uw.edu>
 Version     : 2015.02.1
 Description : 2 methods of use:  main servername port   or main port
             : if a servername is provided, the program will assume to launch
             : the server.  The user will be prompted to indicate the desired
             : protocol.
 ============================================================================
 */

#ifndef MAIN_H
#include "main.h"
#endif

/*******************************************************
 * DETERMINES SERVER OR CLIENT BASED ON ARGUMENTS AND  *
 * PROMPTS THE USER FOR THE DESIRED PROTOCOL.  IF THE  *
 * ARGUMENTS ARE BAD, THE SYSTEM FILL FAIL.            *
 ******************************************************/
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
			case '3':
				if (argc > 2)
					client_rpc_init(hostname);
				else
					server_rpc_init();
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

/********************************************
 * VALIDATES AND EXTRACTS THE HOSTNAME AND  *
 * PORT NUMBER FROM THE COMMAND LINES.  IF  *
 * THE PORT NAME IS BAD, IT WILL RETURN -1  *
 * HOSTNAME AND PORTS ARE STORED AT THE     *
 * ADDRESSES PROVIDED                       *
 * *****************************************/
int validateCommandLine(int argc, char * argv[], char* hostname, unsigned short* port_num)
{
	// CHECK THE NUMBER OF ARGUMENTS.  IF ARG = 2 THEN PORT IS ON INDEX 1, OTHERWISE IT IS ON INDEX 2
	int i;
	if (argc <= 1) //NOT ENOUGH
	{
		printf("USAGE:\n\t tcss558 [hostname|ipaddress] portnum \nEXAMPLE:\n\t To run the client software provide the port number AND the server name: (e.g. tcss558 192.168.1.10 7777) \n\t To run the server software, only provided the port number (e.g. tcss558 7777).\n\n");
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

/**************************************
 * VERIFIES THAT THE PORT PROVIDED IS *
 * AN INTEGER BETWEEN 65534 AND 100   *
 * INCLUSIVE.  RETURNS 0 IF TRUE, AND *
 * -1 IF FALSE                        *
 *************************************/
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

/***************************************
 * DRAWS THE APPROPRIATE MENU BASED ON *
 * THE NUMBER OF ARGUMENTS PROVIDED.   *
 * IF THE NUMBER OF ARGUMENTS IS 2, IT *
 * IS ASSUMED TO BE THE SERVER, AND IF *
 * THERE ARE MORE THAN 2, IT IS ASSUMED*
 * TO BE A CLIENT CONNECTION.  THE     *
 * ADDITIONAL ARGUMENTS ARE IGNORED    *
 * ************************************/
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

	printf("Please Choose a protocol:\n  1. UDP\n  2. TCP\n  3. RPC\n  Q. Quit \n>> ");
}
