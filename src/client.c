#ifndef CLIENT_H
#include "client.h"
#endif


int client_udp_init(char* hostname, unsigned short port_num)
{
	//TODO UPDATE UDP CLIENT (WHY IS THIS NOT COMMITTING1?)
	printf("You ran the UDP client as hostname:port => %s:%d", hostname, port_num);
	exit(0);
}

int client_tcp_init(char* hostname, unsigned short port_num)
{
	printf("You ran the TCP client as hostname:port => %s:%d", hostname, port_num);

	printf("You're testing the TCP Client! \n");
	int server_sock;
	int rc;
	int n;
	int sending;

	struct sockaddr_in server;
	struct sockaddr_in client;
	char message[BUFFSIZE];
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
		printf("The server is connected");
	}

	// PREPARE TO SEND MESSAGES
	int message_count = 21;
	char* messages[message_count];
	getMessages(messages);

	// SEND A BUNCH OF MESSAGES!
	for(int i = 0; i < message_count; i++)
	{
		n = send(server_sock, messages[i], strlen(messages[i]), 0);
		if (n < 0)
			log_write("client.log", hostname, hostname, port_num, "Message Failed", 1);
		else
			log_write("client.log", hostname, hostname, port_num, messages[i], 1);

		bzero(rmessage, BUFFSIZE);
		n = recv(server_sock, rmessage, BUFFSIZE,0);

		if (n < 0)
			log_write("client.log", hostname, hostname, port_num, "Received Failed", 0);
		else
			log_write("client.log", hostname, hostname, port_num, rmessage, 0);
	}


	printf("Messages sent.  Ending communication.");
	close(server_sock);
	exit(0);

}


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

void ClientErrorHandle(char *errorMessage) /* Error handling function */
{
	perror(errorMessage);
	exit(-1);
}
