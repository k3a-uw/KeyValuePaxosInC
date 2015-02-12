/*
 ============================================================================
 Name        : client.c
 Author      : Kevin Anderson <k3a@uw.edu> & Daniel Kristiyanto <danielkr@uw.edu>
 Version     : 2015.01.18
 Description : Connects to the server over the desired protocol.  Messages are passed
 between and logged to a file: client.log
 ============================================================================
 */

#ifndef CLIENT_H
#include "client.h"
#endif

/*******************************************************
 * SENDS A MESSAGE/COMMAND TO THE SERVER PROVIDED AS   *
 * HOSTNAME.  THE RESPONSE FROM THE SERVER IS STORED   *
 * IN THE ADDRESS PROVIDED IN THE VARIABLE 'RESPONSE'  *
 * IF THE COMMAND IS POORLY FORMED, NO MESSAGE WILL    *
 * BE TRANSMITTED.  THIS FUNCTION ALSO MANAGES CALLING *
 * THE LOG_WRITE FUNCTION TO ENSURE TRANSACTIONS ARE   *
 * WRITTEN TO LOG FILES.                               *
 ******************************************************/
int client_rpc_send(char* hostname, int command, xdrMsg * message,
		xdrMsg * response) {
	char s_command[BUFFSIZE];
	switch (command) {
	case RPC_PUT:
		sprintf(s_command, "PUT|Key=%d|Value=%d", message->key, message->value);
		break;
	case RPC_GET:
		sprintf(s_command, "GET|Key=%d", message->key);
		break;
	case RPC_DEL:
		sprintf(s_command, "DEL|Key=%d", message->key);
		break;
	default:
		sprintf(s_command, "Bad Command: Not sent.");
		log_write("client.log", hostname, hostname, 0, s_command, 0);
		return (-1);
		break;
	}

	log_write("client.log", hostname, hostname, 0, s_command, 0);

	int status = callrpc(hostname,
	RPC_PROG_NUM,
	RPC_PROC_VER, command, xdr_rpc, message, xdr_rpc, response);
	if (status < 0)
		sprintf(s_command, "Receive Failed!");
	else
		sprintf(s_command, "Key=%d|Value=%d", response->key, response->value);

	log_write("client.log", hostname, hostname, 0, s_command, 1);
	return status;

}

/***********************************************
 * CALLED BY THE MAIN FUNCTION AND INITIALIZES *
 * COMMUNICATION WITH THE SERVER PROVIDED AS   *
 * HOSTNAME.  THIS MEANS SIMPLY SETTING UP A   *
 * LIST OF COMMANDS AND MAKING CALLS TO        *
 * CLIENT_RPC_SEND                             *
 **********************************************/
int client_rpc_init(char** servers, int server_count) {

	client_ui(servers, server_count);

	//	printf("You are running the RPC Client connecting to hostname => %s\n", hostname);
//	int status;
//
//	xdrMsg response;
//
//	xdrMsg messages[15];
//
//	getRPCMessages(messages);
//
//	int commands[15];
//	getRPCCommands(commands);
//
//	printf("Commands = %d\n", commands[0]);
//	for(int i = 0; i < 15; i++)
//	{
//		status = client_rpc_send(hostname, commands[i], &messages[i], &response);
//		if(response.key == -1)
//			printf("Action cannot be completed.\n");
//		if (status < 0)
//			printf("Message failed!\n");
//	}
//
//	printf("Messages Completed!\n\n\n");

}

int client_ui(char** servers, int server_count) {

	xdrMsg * message;
	xdrMsg * response;
	int status;

	char user_input[128];
	char command_word[16];
	int command;
	while (1) {
		command = -1;
		while (command == -1) {
			printf(
					"Please Choose a Command:\n  1. GET\n  2. PUT\n  3. DEL\n  Q. Quit\n>> ");
			fgets(user_input, 128, stdin);

			switch (user_input[0]) {
			case '1':
				//GET COMMAND;
				command = RPC_GET;
				strcpy(command_word, "Get");
				break;
			case '2':
				//PUT COMMAND;
				command = RPC_PUT;
				strcpy(command_word,"Put");
				break;
			case '3':
				//DELETE COMMAND;
				command = RPC_DEL;
				strcpy(command_word, "Delete");
				break;
			case 'Q':
			case 'q':
				printf("Goodbye!\n");
				exit(0);
			default:
				printf("Invalid Entry, Try Again.");

			}
		}

		int key_command = -1;
		int key_value;
		int is_integer;
		char * toss;
		while (key_command == -1) {
			is_integer = 1;
			key_value = 0;
			printf(
					"Please enter the key (integer) for which you'd like to %s [or q to quit]:\n>> ",
					command_word);
			fgets(user_input, 128, stdin);

			if (user_input[0] == 'q') {
				printf("Goodbye!\n");
				exit(0);
			}

			for (int i = 0; i < 128; i++) {
				if (user_input[i] == '\n') {
					user_input[i] = '\0';
					break;
				}
			}
			// CHECK TO ENSURE IT IS AN INTEGER
			for (int i = 0; i < strlen(user_input); i++) {
				if (!isdigit(user_input[i]) && user_input[i] != '-') {
					is_integer = -1;
					break;
				}
			}

			if (is_integer == 1) {
				key_value = strtol(user_input, &toss, 10);
				if (key_value == -1) {
					printf(
							"I'm sorry but -1 is an invalid sentinel key.  Please use another one!");
				} else {
					key_command = 1;
				}
			}

			if (key_command == -1)
				printf("Invalid Command, try again.\n\n");
		}

		int value_command = -1;
		int value_value = -1;

		if (command == RPC_PUT)  // WE ONLY NEED A VALUE FOR PUTS!
		{
			while (value_command == -1) {
				is_integer = 1;
				printf(
						"Please enter a value that you'd like to enter for key %d [or q to quit]:\n>> ",
						key_value);

				fgets(user_input, 128, stdin);

				if (user_input[0] == 'q') {
					printf("Goodbye!\n");
					exit(0);
				}

				for (int i = 0; i < 128; i++) {
					if (user_input[i] == '\n') {
						user_input[i] = '\0';
						break;
					}
				}

				// CHECK TO ENSURE IT IS AN INTEGER
				for (int i = 0; i < strlen(user_input); i++) {
					if (!isdigit(user_input[i])) {
						is_integer = -1;
						break;
					}
				}

				if (is_integer == 1) {
					char *toss;
					value_value = strtol(user_input, &toss, 10);
					value_command = 1;
				}

				if (value_command == -1)
					printf("Invalid Command, try again.\n\n");
			}
		}

		int server_command = -1;
		int server_value;
		while (server_command == -1) {
			is_integer = 1;
			server_value = 0;
			printf(
					"Please select the server on which to perform the action: \n");
			for (int i = 0; i < server_count; i++)
				printf("  %d. %s\n", i + 1, servers[i]);
			printf("\n  Q. Quit\n >> ");

			fgets(user_input, 128, stdin);

			if (user_input[0] == 'q') {
				printf("Goodbye!\n");
				exit(0);
			}

			for (int i = 0; i < 128; i++) {
				if (user_input[i] == '\n') {
					user_input[i] = '\0';
					break;
				}
			}

			// CHECK TO ENSURE IT IS AN INTEGER
			for (int i = 0; i < strlen(user_input); i++) {
				if (!isdigit(user_input[i])) {
					is_integer = -1;
					break;
				}
			}

			if (is_integer == 1) {
				server_value = strtol(user_input, &toss, 10);

				if (server_value >= 1 && server_value <= server_count) {
					server_value = server_value - 1;  // get adjust the index!
					server_command = 1;
				} else {
					printf("Invalid Server Entry, try again.");
				}
			}
		}

		printf(
				"I'm going to execute a %s with key %d and value %d (if applicable) on server %s.\n",
				command_word, key_value, value_value, servers[server_value]);

		message->key = key_value;
		message->value = value_value;

		status = client_rpc_send(servers[server_value], command, message,
				response);

		if (status < 0)
			printf("Message failed!\n");
		else
			printf("Send Successful!\n Try again!");
	} // LOOP BACK AROUND AND TRY AGAIN!

}

/********************************************************
 *  LAUNCHES A SCRIPT THAT COMMUNICATES WITH THE SERVER *
 *  INDICATED, OVER THE PORT_NUMBER PROVIDED.  CALLS    *
 *  getMessages() TO DETERMINE WHAT COMMANDS TO SEND TO *
 *  THE SERVER. EXITS PROGRAM WHEN COMPLETED.           *
 *  THE PROTOCOL USED: UDP
 ********************************************************/
int client_udp_init(char* hostname, unsigned short port_num) {
	printf("You ran the UDP client as hostname:port => %s:%d\n", hostname,
			port_num);

	int server_sock;
	int n;

	struct sockaddr_in server;
	char rmessage[BUFFSIZE];

	int message_count = 21;
	char * messages[message_count];
	getMessages(messages);

	server_sock = socket(AF_INET, SOCK_DGRAM, 0);

	if (server_sock < 0)
		ClientErrorHandle("Socket() Failed");

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(hostname);
	server.sin_port = htons(port_num);

	int server_length = sizeof(server);
	for (int i = 0; i < message_count; i++) {
		n = sendto(server_sock, messages[i], strlen(messages[i]), 0,
				(struct sockaddr *) &server, server_length);
		if (n < 0) {
			log_write("client.log", hostname, hostname, port_num,
					"Message Failed", 0);
			ClientErrorHandle("Message Send Failure");
		} else {
			log_write("client.log", hostname, hostname, port_num, messages[i],
					0);
		}

		bzero(rmessage, BUFFSIZE);
		n = recvfrom(server_sock, rmessage, BUFFSIZE, 0,
				(struct sockaddr *) &server, &server_length);
		if (n < 0)
			log_write("client.log", hostname, hostname, port_num,
					"Response Failed", 1);
		else
			log_write("client.log", hostname, hostname, port_num, rmessage, 1);
	}

	printf("Messages sent.  Ending communication.");
	close(server_sock);

	exit(0);

}

/********************************************************
 *  LAUNCHES A SCRIPT THAT COMMUNICATES WITH THE SERVER *
 *  INDICATED, OVER THE PORT_NUMBER PROVIDED.  CALLS    *
 *  getMessages() TO DETERMINE WHAT COMMANDS TO SEND TO *
 *  THE SERVER. EXITS PROGRAM WHEN COMPLETED.           *
 *  THE PROTOCOL USED: TCP
 ********************************************************/
int client_tcp_init(char* hostname, unsigned short port_num) {
	printf("You ran the TCP client as hostname:port => %s:%d", hostname,
			port_num);

	printf("You're testing the TCP Client! \n");
	int server_sock;
	int rc;
	int n;

	struct sockaddr_in server;
	char rmessage[BUFFSIZE];

	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock < 0)
		ClientErrorHandle("Socket Failed");

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(hostname);
	server.sin_port = htons(port_num);

	rc = connect(server_sock, (struct sockaddr *) &server,
			sizeof(struct sockaddr));
	if (rc < 0) {
		ClientErrorHandle("Cannot Connect to Server");
		exit(-1);
	} else {
		printf("The server is connected.\n");
	}

	// PREPARE TO SEND MESSAGES
	int message_count = 21;
	char* messages[message_count];
	getMessages(messages);

	// SEND A BUNCH OF MESSAGES!
	for (int i = 0; i < message_count; i++) {

		n = send(server_sock, messages[i], BUFFSIZE, 0);
		if (n < 0)
			log_write("client.log", hostname, hostname, port_num,
					"Message Failed", 0);
		else
			log_write("client.log", hostname, hostname, port_num, messages[i],
					0);

		bzero(rmessage, BUFFSIZE);

		n = recv(server_sock, rmessage, BUFFSIZE, MSG_WAITALL);
		if (n < 0)
			log_write("client.log", hostname, hostname, port_num,
					"Received Failed", 1);
		else
			log_write("client.log", hostname, hostname, port_num, rmessage, 1);
	}

	printf("Messages sent.  Ending communication.");
	close(server_sock);
	exit(0);

}

/*****************************************************
 * SAVES A CHAR* ARRAY THAT CONTAINTS A SERIES OF    *
 * MESSAGES THAT ARE TO BE SENT TO THE SERVER        *
 * **************************************************/
void getMessages(char* messages[]) {
	messages[0] = "0|1|1234|";  //first put
	messages[1] = "0|2|78890|";  //second put
	messages[2] = "0|1|2345|";  //try and put over 1 again, will get back error;
	messages[3] = "1|1|";       //will get back 1234;
	messages[4] = "1|2|";       //will get back 78890
	messages[5] = "2|1|";       //will delete key 1;
	messages[6] = "1|1|";       //try and get one, it wont' exist
	messages[7] = "3|12|123|";  //unknown command;
	messages[8] = "Put:This"; //unknown command;
	messages[9] = "0|5|55555";  //another put
	messages[10] = "1|5";        //get 55555
	messages[11] = "0|10|111010";  //another put
	messages[12] = "0|11|1919";  //another put
	messages[13] = "0|12|1212";  //another put
	messages[14] = "0|13|1313";  //another put
	messages[15] = "1|13|";  //gets 1313
	messages[16] = "1|10|";  //gets 111010
	messages[17] = "1|12|";  //gets1212
	messages[18] = "2|13|";  //deletes 13
	messages[19] = "2|10|"; //deletes 10
	messages[20] = "1|10|";  //tries to get 10 but fails.

	return;
}

/************************************************
 * SAVES AN ARRAY OF MESSAGES TO BE SENT TO THE *
 * SERVER AS A DEMO OF THE SYSTEM WORKING       *
 * AND IS TO BE USED IN CONJUNCTION WITH THE    *
 * FUNCTION GETRPCCOMMANDS.                     *
 ***********************************************/
void getRPCMessages(xdrMsg* messages) {
	messages[0].key = 1;
	messages[0].value = 100;
	messages[1].key = 1;
	messages[1].value = 200;
	messages[2].key = 2;
	messages[2].value = 300;
	messages[3].key = 1;
	messages[4].key = 3;
	messages[5].key = 2;
	messages[6].key = 2;
	messages[7].key = 3;
	messages[7].value = 999;
	messages[8].key = 3;
	messages[9].key = 3;
	messages[10].key = 9;
	messages[11].key = 999;
	messages[11].value = 1234;
	messages[12].key = 999;
	messages[13].key = 999;
	messages[14].key = 999;

	return;

}

/************************************************
 * SAVES AN ARRAY OF COMMANDS TO BE SENT TO THE *
 * SERVER AS A DEMO OF THE SYSTEM WORKING       *
 * AND IS TO BE USED IN CONJUNCTION WITH THE    *
 * FUNCTION GETRPCMESSAGES.                     *
 ***********************************************/
void getRPCCommands(int* commands) {
	commands[0] = RPC_PUT;
	commands[1] = RPC_PUT;
	commands[2] = RPC_PUT;
	commands[3] = RPC_GET;
	commands[4] = RPC_GET;
	commands[5] = RPC_DEL;
	commands[6] = RPC_GET;
	commands[7] = RPC_PUT;
	commands[8] = RPC_DEL;
	commands[9] = RPC_GET;
	commands[10] = RPC_DEL;
	commands[11] = RPC_PUT;
	commands[12] = RPC_GET;
	commands[13] = RPC_DEL;
	commands[14] = RPC_DEL;

}

/*********************************************
 * WRITES ERROR TO THE CONSOLE AND EXITS THE *
 * SYSTEM WITH AN ERROR CODE OF -1           *
 ********************************************/
void ClientErrorHandle(char *errorMessage) {
	perror(errorMessage);
	exit(-1);
}
