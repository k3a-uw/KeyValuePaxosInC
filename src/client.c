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





int client_rpc_init(char* hostname)
{
	printf("You are running the RPC Client connecting to hostname => %s\n", hostname);

	char response[BUFFSIZE];

	int status;
	unsigned long tmpResponse;
	unsigned long message = 18;


	printf("Calling to RPC...\n");
	status = callrpc(hostname,   //host
				1234,      // program number
				123,		// version number
				12,		//process number
				xdr_u_long,  // message datatype
				&message,   // as message
				xdr_u_long,  // response datatype
				&tmpResponse
				);
	printf("Status = %d", status);
	printf("Response = %d\n", tmpResponse);
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

/*********************************************
 * WRITES ERROR TO THE CONSOLE AND EXITS THE *
 * SYSTEM WITH AN ERROR CODE OF -1           *
 ********************************************/
void ClientErrorHandle(char *errorMessage) /* Error handling function */
{
	perror(errorMessage);
	exit(-1);
}
