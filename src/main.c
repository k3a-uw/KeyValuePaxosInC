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
	int actAs;
	actAs = validateCommandLine(argc, argv);


	//run as
	if(actAs==1) //server
	{
		//run server code here
	}
	else if (actAs==2) //client
	{
		while(1){
			int dest = printMenu(1); // Choose the destination server
			switch(printMenu(2)) //change later
			{
			case 1:
				// PUT
				break;
			case 2:
				// get
				break;
			case 3:
				//DELETE
				break;
			case 4:
				// SCRIPTED
				client_rpc_init("localhost");
				break;
			case 5:
				// QUIT
				printf("Program will now exit.");
				exit(0);
				break;
			default:
				printf("Invalid input. Try again.");
				break;
			}

		}
	}
	else
	{
		printf("Bye.");
		exit(-1);
	}

	/*
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
	 */
}

/********************************************
 * VALIDATES AND EXTRACTS THE HOSTNAME AND  *
 * PORT NUMBER FROM THE COMMAND LINES.  IF  *
 * THE PORT NAME IS BAD, IT WILL RETURN -1  *
 * HOSTNAME AND PORTS ARE STORED AT THE     *
 * ADDRESSES PROVIDED                       *
 * *****************************************/
int validateCommandLine(int argc, char * argv[])
{
	// CHECK THE NUMBER OF ARGUMENTS.  IF ARG = 2 THEN PORT IS ON INDEX 1, OTHERWISE IT IS ON INDEX 2
	/*
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
	*/
	int i=0;
	char arg[100];

	if (argc <= 1)
	{
		strcpy(arg,"client"); //DEFAULT TO CLIENT
	}
	else
		strcpy(arg,argv[1]);

	if(strcmp(arg,"client")==0)
	{
		i=2;
		printf("Will run as a client.\n");
	}
	else if(strcmp(arg,"server")==0)
	{
		i=1;
		strcpy(arg,argv[1]);
		printf("Will run as a server.\n");
	}
	else
	{
		printf("Program doesn't know how to act. Usage:\n For Server: tcss558 server \n For Client: tcss558 client \nProgram will now exit");
		exit(-1);
	}
	return i;

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
int printMenu(int argc)
{
	/*
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
	*/

	int user_input=-1;

	if(argc==1)
	{
		int i=0;
		// Later will be replace with the function to read serverlist.txt
		char* serverList[5]={"n01","n02","n03","n04","n05"};

		printf("List of available server:\n");

		for(i=0;i<sizeof(serverList);i++)
			{
				printf("%i. %s\n", i+1, serverList[i]);
			}
		while(!((0<user_input) && (user_input < sizeof(serverList))))
		{
		printf("Please select server you like to work with:");
		//fgets(user_input, 128, stdin);
		}
		return user_input;
	}
}

	/*
void readServerList()
{

	int lines_allocated = 128;
	int max_line_len = 100;
	int i,j;

	// Allocate lines of text

	char **srvList = (char **)malloc(sizeof(char*)*lines_allocated);
	if (srvList==NULL)
	{
		fprintf(stderr,"Out of memory (1).\n");
		exit(-1);
	}

	FILE *fp = fopen("serverlist.txt", "r");
	if (fp == NULL)
	{
		fprintf(stderr,"File serverlist.txt not found.\nProgram will now exit.\n");
		exit(-1);
	}

	for (i=0;1;i++)
	{
		// Check allocation
		if (i >= lines_allocated)
		{
			int new_size;
			// Double  allocation and re-allocate
			new_size = lines_allocated*2;
			srvList = (char **)realloc(srvList,sizeof(char*)*new_size);
			if (srvList==NULL)
			{
				fprintf(stderr,"Error reading serverlist.txt\n");
				exit(-1);
			}
			lines_allocated = new_size;
		}


		// Allocate space for the next line
		srvList[i] = malloc(max_line_len);
		if (srvList[i]==NULL)
		{
			fprintf(stderr,"Error reading serverlist.txt\n");
			exit(-1);
		}
		if (fgets(srvList[i],max_line_len-1,fp)==NULL)
			break;

		// Get rid of CR or LF at end of line
		for (j=strlen(srvList[i])-1;j>=0 && (srvList[i][j]=='\n' || srvList[i][j]=='\r');j--);
		srvList[i][j]='\0';
		}

	    j=0;
	    for(j = 0; j < i; j++)
	        printf("%s\n", srvList[j]);

	    //return srvList;
}
*/
