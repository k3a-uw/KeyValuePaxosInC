
#ifndef TCP_CLIENT_H
  #include "tcp_client.h"
#endif

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

int tcp_client_main(int argc, char *argv[])
{
	printf("You're testing the TCP Client! \n");
	 int sockfd,rc, n,sending;
	 struct sockaddr_in serveraddress,clientaddress;
	 char message[20];
	 char rmessage[20];

	 if (argc != 3)
	 {
		 printf("usage:  tcpclient [IP address] [portnumber]\n Program will now exit.");
	     exit(1);
	 }

	 sockfd=socket(AF_INET,SOCK_STREAM,0);

	 bzero(&serveraddress,sizeof(serveraddress));
	 serveraddress.sin_family = AF_INET;
	 serveraddress.sin_addr.s_addr=inet_addr(argv[1]);
	 serveraddress.sin_port=htons(argv[2]);

	 rc=connect(sockfd, (struct sockaddr *)&serveraddress, sizeof(struct sockaddr));
	 if(rc<0)
	 {
		 printf("Connection Error\nIs the server ready?");
	 }
	 while(1)
	 {

	 }


}
