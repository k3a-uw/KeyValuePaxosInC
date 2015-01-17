

#ifndef UDP_SERVER_H
	#include "udp_server.h"
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

int udp_server_main(int argc, char *argv[])
{
	printf("You're testing the UDP Server!\n");


	int sockfd, rc, len, n;
	struct sockaddr_in serveraddress;
	struct sockaddr_in clientaddress;
	char message[20];

	// AF INET, to receive incoming connection
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if (sockfd<0)
	{
		printf("Error setting the socket \n");
		exit(-1);
	}
	else
	{
		printf("Setting the Socket \n");
	}
	 // BIND IT
	bzero(&serveraddress, sizeof(serveraddress));
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddress.sin_port = htons(argv[1]);
	rc = bind(sockfd, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
	if (rc<0)
	{
		printf("Binding Error. \n");
		close(sockfd);
		exit(-1);
	}else
	{
		printf("Binding at port: %s \n", argv[1]);
	}
	while(1)
	{
		len = sizeof(clientaddress);
		n = recvfrom(sockfd,message,1000,0,(struct sockaddr *)&clientaddress,&len);
		sendto(sockfd,message,n,0,(struct severaddress *)&clientaddress,sizeof(clientaddress));
		message[n] = 0;
		printf("%s",message);
	 }
	printf("finished");


}
