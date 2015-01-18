/*
 * udp_client.c
 *
 *  Created on: Jan 12, 2015
 *      Author: kevanderson
 */

#ifndef UDP_CLIENT_H
	#include "udp_client.h";
#endif
#ifndef SOCKET_H
	#include <sys/socket.h>
#endif
#ifndef IN_H
	#include <netinet/in.h>
#endif
#ifndef STDIO_H
	#include <stdio.h>
#endif
#define BUFFLEN 20

int udp_client_main(int argc, char *argv[])
{
	printf("You're testing the UDP Client!\n");

   int sockfd,n,serverlength,sending;
   struct sockaddr_in serveraddress,clientaddress;
   char message[BUFFLEN];
   char rmessage[BUFFLEN];

   //Checking the arguments
   if (argc != 3)
   {
      printf("usage:  udpclient [IP address] [portnumber]\n Program will now exit.");
      exit(1);
   }

   // Port Filter from KA
   	long port_l;
   	unsigned short port;
   	char *toss;
   	port_l = strtol(argv[2], &toss, 10);
   	if (port_l < 65535 && port_l >= 1000)  // we have a good port
   	{
   		port = (unsigned short) port_l;
   		printf("Port is %d\n", port);
   	} else {
   		printf("The port is invalid.  Please provide a port between 1000 and 65534 (inclusive).");
   		return(-1);
   	}

   //Setting the socket
   sockfd=socket(AF_INET,SOCK_DGRAM,0);
   bzero(&serveraddress,sizeof(serveraddress));
   serveraddress.sin_family = AF_INET;
   serveraddress.sin_addr.s_addr=inet_addr(argv[1]);
   serveraddress.sin_port=htons(port);
   printf("Waiting to receive message from %s \n",argv[1]);
   while (1)
   {
	  // Sending message to server
	  serverlength=sizeof(serveraddress);
	  printf("Command: ");
      sending=sendto(sockfd,message,strlen(message),0,
             (struct sockaddr *)&serveraddress,serverlength);
      if (sending<0)
      {
    	  printf("Error sedning message");
    	  close(sendto);
      }
      // Receiving message from Server
      n=recvfrom(sockfd,rmessage, 1000,0,(struct sockaddr *) &serveraddress, sizeof(serveraddress));
      if(n<0)
      {
    	  close(recvfrom);
      }
      rmessage[n]=0;
      //Display it or do some calculation
      //fputs(rmessage,stdout);
   }

   return 0;
}
