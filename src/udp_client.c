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


int udp_client_main(int argc, char *argv[])
{
	printf("You're testing the UDP Client!\n");

   int sockfd,n,sending;
   struct sockaddr_in serveraddress,clientaddress;
   char message[20];
   char rmessage[20];

   //Checking the arguments
   if (argc != 3)
   {
      printf("usage:  udpclient [IP address] [portnumber]\n Program will now exit.");
      exit(1);
   }

   //Setting the socket
   sockfd=socket(AF_INET,SOCK_DGRAM,0);
   bzero(&serveraddress,sizeof(serveraddress));
   serveraddress.sin_family = AF_INET;
   serveraddress.sin_addr.s_addr=inet_addr(argv[1]);
   serveraddress.sin_port=htons(argv[2]);
   printf("Ready to receive message at port %s", argv[2]);

   while (1)
   {
	  printf("receiving message from %s \n",argv[1]);
	  // Sending message to server
      sending=sendto(sockfd,message,strlen(message),0,
             (struct sockaddr *)&serveraddress,sizeof(serveraddress));
      if (sending<0)
      {
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
