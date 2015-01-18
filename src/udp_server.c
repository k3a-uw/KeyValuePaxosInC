

#ifndef UDP_SERVER_H
	#include "udp_server.h"
#endif

#ifndef STDIO_H
	#include <stdio.h>
#endif

#ifndef stdlib
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
#include <netdb.h>
#include "keyvalue.h"
#include "log.h"

#define BUFLEN 128

int udp_server_main(int argc, char *argv[])
{
	kv* kv_store = kv_new();
	printf("You're testing the UDP Server!\n");

	int sockfd, rc, n, clientlength, response, result;
	struct sockaddr_in serveraddress;
	struct sockaddr_in clientaddress;
	char message[BUFLEN];	//received message
	char rmessage[BUFLEN]; 	//response message

	if (argc !=2)
		{
			printf("Usage udpserver [portnumber] \n");
		}

	// Port Filter from KA
	long port_l;
	unsigned short port;
	char *toss;
	port_l = strtol(argv[1], &toss, 10);
	if (port_l < 65535 && port_l >= 1000)  // we have a good port
	{
		port = (unsigned short) port_l;
		printf("Port is %d\n", port);
	} else {
		printf("The port is invalid.  Please provide a port between 1000 and 65534 (inclusive).");
		return(-1);
	}


	// SOCK_DGRAM, initiate sock in UDP
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if (sockfd<0)
	{
		printf("Error establishing socket \n");
		exit(-1);
	}
	else
	{
		printf("Socket established\n");
	}


	 // BIND IT
	bzero(&serveraddress, sizeof(serveraddress));
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddress.sin_port = htons(port);
	rc = bind(sockfd, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
	if (rc<0)
	{
		printf("Binding Error. \n Maybe try different port?");
		close(sockfd);
		exit(-1);
	}

	while(1){
		printf("Ready to serve...\n");
		//receiving messages
		clientlength=sizeof(struct sockaddr_in);
		n = recvfrom(sockfd,message,BUFLEN,0,(struct sockaddr*)&clientaddress,&clientlength);
		message[n]=0;
		printf("%s received, ",message);


		// KA'S LOG HANDLER

	    struct hostent *client_name;
		client_name = gethostbyaddr((const char *)&clientaddress.sin_addr.s_addr,sizeof(clientaddress.sin_addr.s_addr), AF_INET);

		char *client_ip;
		client_ip = inet_ntoa(clientaddress.sin_addr);

		if ((client_ip == NULL)||(client_name == NULL))
		{
			printf("Host unknown. Program will now exit.");
			exit(-1);
		}

		log_write("server.log", client_name->h_name, client_ip, port, message, 0);


		//message operations here
		//strcpy(rmessage,message);
		result = handle_message(kv_store, message, rmessage);

		//responding to message
		printf("responding with %s.\n",rmessage);
		int reponse = sendto(sockfd,rmessage,strlen(rmessage),0,(struct sockaddr *)&clientaddress,clientlength);
		if(response<0)
		{
			printf("Error replying");
			exit(-1);
		}
		log_write("server.log", client_name->h_name, client_ip, port, message, 1);
	}
	printf("finished");
	close(sockfd);
}

