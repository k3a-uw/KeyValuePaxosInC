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

#ifndef STDIO_H
	#include <stdio.h>
#endif

#ifndef TYPES_H
	#include <sys/types.h>
#endif

#ifndef SOCKET_H
	#include <sys/socket.h>
#endif

#ifndef NETINET_H
	#include <netinet/in.h>
#endif

#ifndef INET_H
	#include <arpa/inet.h>
#endif


int tcp_server_main(int argc, char *argv[])
{
	printf("You're testing the TCP Server!\n");
	int sockfd, rc, clientlength, accpt, n, proc;
	struct sockaddr_in serveraddress;
	struct sockaddr_in clientaddress;
	char message[20];

	if(argc!=2)
	{
		printf("Usage tcpserver [portnumber]");
		exit(-1);
	}
	// Setting the socket
	sockfd=socket(AF_INET, SOCK_STREAM,0);
	if (sockfd<0)
	{
		printf("Error establishing socket.");
		close(socket);
	}
	bzero(&serveraddress,sizeof(serveraddress));
	serveraddress.sin_family=AF_INET;
	serveraddress.sin_addr.s_addr=htonl(INADDR_ANY);
	serveraddress.sin_port=htons(argv[1]);
	//Binding
	rc=bind(sockfd,(struct sockaddr *)&serveraddress,sizeof(serveraddress));
	if(rc<0)
	{
		printf("Binding error. \nMaybe try different port?\n");
		close(bind);
		exit(-1);
	}
	//Listen
	listen(sockfd,1024);
	clientlength=sizeof(struct sockaddr_in);
	while(1){
		accpt=accept(sockfd,(struct sockaddr *)&clientaddress,&clientlength);
		if(accpt<0)
		{
			printf("Error: Accepting \n");
			exit(-1);
		}
		printf("Accepting Connection...");
		if((proc = fork()) ==0)
		{
			close(sockfd);
			for(;;)
			{
				n = recvfrom(sockfd,message,1000,0,(struct sockaddr *)&clientaddress,sizeof(clientaddress));
				sendto(sockfd,message,n,0,(struct sockaddr *)&clientaddress,sizeof(clientaddress));
				message[n] = 0;
			}

		}
	}
	close(sockfd);



}
