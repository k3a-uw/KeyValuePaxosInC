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
	// FIRST READ THE SERVER FILE
	int server_count;
	char** server_list[MAX_SERVERS];
	for(int i = 0; i < MAX_SERVERS; i++)
		server_list[i] = malloc(HOST_NAME_LENGTH);

	FILE* fd = fopen("./serverlist.txt","r");
	if (fd == NULL)
	{
		printf("Cannot find file serverlist.txt, please create the file and try again.\n");
		exit(-1);
	}


	// BUILD THE STRING OF SERVER NAMES
	int i = 0;
	char line[HOST_NAME_LENGTH];
	while (fgets(line, sizeof(line), fd) && i < MAX_SERVERS)
	{
		// TRIM THE STRING -- REPLACE \N WITH \0
		for(int j = 0; j < strlen(line); j++)
		{
			if (line[j] == '\n')
			{
				line[j] = '\0';
				break;
			}
		}
		strcpy(server_list[i], line);
		i++;
	}

	server_count = i;


	//NOW VALIDATE THE ARGUMENTS AND CALL THE PROPER FUNCTION

	if (argc < 2)  // MUST HAVE AT LEAST ONE ADDITIONAL ARG
	{
		printf("Usage: tcss558 client|server\n");
		exit(-1);
	} else if (strcmp(argv[1],"server") == 0) {
		printf("Running as Server...\n");
		server_rpc_init(server_list, server_count);
	} else if (strcmp(argv[1], "client") == 0) {
		client_rpc_init(server_list, server_count);
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

