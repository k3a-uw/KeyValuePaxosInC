/*
 ============================================================================
 Name        : server.c
 Author      : Kevin Anderson <k3a@uw.edu> & Daniel Kristiyanto <danielkr@uw.edu>
 Version     : 2015.01.10
 Description : TCSS558 Project 1
 ============================================================================
 */

#ifndef TCP_SERVER_H
#include "tcp_server.h"
#endif

#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <string.h>     /* for memset() */
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#define MAXPENDING 5    /* Maximum outstanding connection requests */

void DieWithError(char *errorMessage) /* Error handling function */
{
	perror(errorMessage);
//	printf("Error: %s", errorMessage);
	exit(-1);
}


int tcp_server_main(int argc, char *argv[])
{

	printf("You're testing the TCP Server!\n");

	long port_l;
	unsigned short port;
	char *toss;
	port_l = strtol(argv[1], &toss, 10);


	if (port_l < 65535 && port_l >= 1000)  // we have a good port
	{
		port = (unsigned short) port_l;
		printf("Port is %d", port);
	} else {
		printf("The port is invalid.  Please provide a port between 1000 and 65534 (inclusive).");
		return(-1);
	}

    struct sockaddr_in server;

    /* Create socket for incoming connections */
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
        DieWithError("socket() failed");

    /* Construct local address structure */
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    /* Bind to the local address */
    int bindsock = bind(sock, (struct sockaddr *) &server, sizeof(server));
    if (bindsock < 0)
    	DieWithError("bind() failed");

    /* Listen to the incoming socket */
    int listensock = listen(sock, MAXPENDING);
    if (listensock < 0)
    	DieWithError("listen() failed");

    int client_sock;
    struct sockaddr_in client;
    int client_len = sizeof(client);
    int sendsock;
    char *message = "No Message!\n";
    while(1)
    {
    	client_sock = accept(sock, (struct sockaddr *) &client, &client_len);
    	if (client_sock == -1)
    		DieWithError("accept() failed");

    	int messageLength = 128;
    	char message[messageLength+1];
    	int in;
    	int i;
    	int limit = messageLength;

    	recv(client_sock, &message, messageLength, 0);
    	printf("Message was: %s", message);

//    	sendsock = send(client_sock, message, strlen(message), 0);
//    	if (sendsock == -1)
//    		DieWithError("send() failed");
//
//    printf("IP of client: %s\n" , (char *) inet_ntoa(client.sin_addr));
//    printf("Bytes Sent: %d", sendsock);

    close(client_sock);
    }

    return sock;
}
