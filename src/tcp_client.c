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

#include <stdlib.h>
#include <string.h>
#define BUFFSIZE 128

int tcp_client_main(int argc, char *argv[]) {
	printf("You're testing the TCP Client! \n");
	int server_sock;
	int rc;
	int n;
	int sending;

	struct sockaddr_in server;
	struct sockaddr_in client;
	char message[BUFFSIZE];
	char rmessage[BUFFSIZE];

	if (argc != 3) {
		printf("usage:  tcpclient [IP address] [portnumber]\n Program will now exit.");
		exit(-1);
	}

	long port_l;
	unsigned short port;
	char *toss;
	port_l = strtol(argv[2], &toss, 10);

	if (port_l < 65535 && port_l >= 1000)  // we have a good port
			{
		port = (unsigned short) port_l;
	} else {
		printf(
				"The port is invalid.  Please provide a port between 1000 and 65534 (inclusive).");
		return (-1);
	}

	server_sock = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_port = htons(port);

	rc = connect(server_sock, (struct sockaddr *) &server, sizeof(struct sockaddr));
	if (rc < 0) {
		printf("Connection Error\nIs the server ready?");
		exit(-1);
	} else {
		printf("The server is connected");
	}

	sprintf(message, "1|1|1234|");
	n = send(server_sock, message, strlen(message), 0);

	if (n < 0)
	{
		printf("There was an error");
		exit(-1);
	}

	n = recv(server_sock, rmessage, BUFFSIZE,0);

	printf("Received: %s", rmessage);


}
