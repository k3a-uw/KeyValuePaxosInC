

#ifndef UDP_SERVER_H
#include "udp_server.h"
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define SERVER_PORT 7777

int udp_server_main(int argc, char *argv[])
{
	//printf("You're testing the UDP Server!\n");
	int sockfd, listening, rc,accepting;
	struct sockaddr_in serveraddress;

	// AF INET, to receviee incoming connection
	listening=socket(AF_INET,SOCK_DGRAM,0);
	if (listen<0)
	{
		printf("Error");
		exit(-1);
	}
	 // BIND IT
	memset(&serveraddress,0, sizeof(serveraddress));
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddress.sin_port = htons(SERVER_PORT);
	rc = bind(listening, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
	if (bind<0)
	{
		print("Error");
		close(listening);
		exit(-1);
	}
	rc = listen(listening, 5);
	if (rc<0)
	{
		print("Error");
		close(listening);
		exit(-1);
	}

	while(1)
	{
		accepting = accept(listening, NULL, NULL);
		if (listening<0)
		{
			print("Error");
			close(listening);
			exit(-1);
		}
	}

	return 0;

}
