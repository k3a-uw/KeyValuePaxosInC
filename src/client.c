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


int client_rpc_send(char* hostname, int command, xdrMsg * message, xdrMsg * response)
{
	char s_command[BUFFSIZE];
	switch (command)
	{
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
		break;
	}


	log_write("client.log", hostname, hostname, 0, s_command, 0);

	int status = callrpc(hostname,
						RPC_PROG_NUM,
						RPC_PROC_VER,
						command,
						xdr_rpc,
						message,
						xdr_rpc,
						response);
	if (status < 0)
		sprintf(s_command, "Receive Failed!");
	else
		sprintf(s_command, "Key=%d|Value=%d",response->key, response->value);

	log_write("client.log", hostname, hostname, 0, s_command, 1);
	return status;

}


int client_rpc_init(char* hostname)
{
	printf("You are running the RPC Client connecting to hostname => %s\n", hostname);
	int status;

	xdrMsg response;

	xdrMsg messages[15];

	getRPCMessages(messages);

	int commands[15];
	getRPCCommands(commands);

	printf("Commands = %d\n", commands[0]);
	for(int i = 0; i < 15; i++)
	{
		status = client_rpc_send(hostname, commands[i], &messages[i], &response);
		if(response.key == -1)
			printf("Action cannot be completed.\n");
		if (status < 0)
			printf("Message failed!\n");
	}

	printf("Messages Completed!\n\n\n");


}



/********************************************************
 *  LAUNCHES A SCRIPT THAT COMMUNICATES WITH THE SERVER *
 *  INDICATED, OVER THE PORT_NUMBER PROVIDED.  CALLS    *
 *  getMessages() TO DETERMINE WHAT COMMANDS TO SEND TO *
 *  THE SERVER. EXITS PROGRAM WHEN COMPLETED.           *
 *  THE PROTOCOL USED: UDP
 ********************************************************/
int client_udp_init(char* hostname, unsigned short port_num)
{
	printf("You ran the UDP client as hostname:port => %s:%d\n", hostname, port_num);

   int server_sock;
   int n;

   struct sockaddr_in server;
   char rmessage[BUFFSIZE];

   int message_count = 21;
   char * messages[message_count];
   getMessages (messages);

   server_sock = socket(AF_INET, SOCK_DGRAM,0);

   if(server_sock < 0)
	   ClientErrorHandle("Socket() Failed");

   bzero(&server, sizeof(server));
   server.sin_family = AF_INET;
   server.sin_addr.s_addr = inet_addr(hostname);
   server.sin_port = htons(port_num);


   int server_length = sizeof(server);
   for (int i = 0; i < message_count; i++)
   {
	   n = sendto(server_sock, messages[i],strlen(messages[i]), 0 , (struct sockaddr *) &server, server_length);
	   if (n < 0)
	   {
		   log_write("client.log", hostname, hostname, port_num, "Message Failed", 0);
		   ClientErrorHandle("Message Send Failure");
	   } else {
		   log_write("client.log", hostname, hostname, port_num, messages[i], 0);
	   }

	   bzero(rmessage, BUFFSIZE);
	   n = recvfrom(server_sock, rmessage, BUFFSIZE, 0, (struct sockaddr *) &server, &server_length);
	   if (n < 0)
		   log_write("client.log", hostname, hostname, port_num, "Response Failed",1);
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
int client_tcp_init(char* hostname, unsigned short port_num)
{
	printf("You ran the TCP client as hostname:port => %s:%d", hostname, port_num);

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

	rc = connect(server_sock, (struct sockaddr *) &server, sizeof(struct sockaddr));
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
	for(int i = 0; i < message_count; i++)
	{

		n = send(server_sock, messages[i], BUFFSIZE, 0);
		if (n < 0)
			log_write("client.log", hostname, hostname, port_num, "Message Failed", 0);
		else
			log_write("client.log", hostname, hostname, port_num, messages[i], 0);

		bzero(rmessage, BUFFSIZE);

		n = recv(server_sock, rmessage, BUFFSIZE,MSG_WAITALL);
		if (n < 0)
			log_write("client.log", hostname, hostname, port_num, "Received Failed", 1);
		else
			log_write("client.log", hostname, hostname, port_num, rmessage, 1);
	}


	printf("Messages sent.  Ending communication.");
	close(server_sock);
	exit(0);

}

/*****************************************************
 * RETURNS A CHAR* ARRAY THAT CONTAINTS A SERIES OF  *
 * MESSAGES THAT ARE TO BE SENT TO THE SERVER        *
 * **************************************************/
void getMessages(char* messages[])
{
	messages[0]  = "0|1|1234|";  //first put
	messages[1]  = "0|2|78890|";  //second put
	messages[2]  = "0|1|2345|";  //try and put over 1 again, will get back error;
	messages[3]  = "1|1|";       //will get back 1234;
	messages[4]  = "1|2|";       //will get back 78890
	messages[5]  = "2|1|";       //will delete key 1;
	messages[6]  = "1|1|";       //try and get one, it wont' exist
	messages[7]  = "3|12|123|";  //unknown command;
	messages[8]  = "Put:This"; //unknown command;
	messages[9]  = "0|5|55555";  //another put
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

void getRPCMessages(xdrMsg* messages)
{
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

void getRPCCommands(int* commands)
{
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
void ClientErrorHandle(char *errorMessage) /* Error handling function */
{
	perror(errorMessage);
	exit(-1);
}
