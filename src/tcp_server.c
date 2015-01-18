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

int handle_message(kv* kv_store, char* msg, char* response);

void DieWithError(char *errorMessage) /* Error handling function */
{
	perror(errorMessage);
	exit(-1);
}



int tcp_server_main(int argc, char *argv[])
{
	// CREATE A NEW KEY VALUE STORE
	kv* kv_store = kv_new();
	printf("You're testing the TCP Server!\n");

	long port_l;
	unsigned short port;
	char *toss;
	port_l = strtol(argv[1], &toss, 10);


	if (port_l < 65535 && port_l >= 1000)  // we have a good port
	{
		port = (unsigned short) port_l;
		printf("Bind to port %d\n", port);
	} else {
		printf("The port is invalid.  Please provide a port between 1000 and 65534 (inclusive).");
		return(-1);
	}

    struct sockaddr_in server;

    /* Create socket for incoming connections */
    int sock = socket(AF_INET, SOCK_STREAM, 0);
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

    char buf[BUFFSIZE]; /* message buffer */
    char response[BUFFSIZE];
    char log[1024] = "log";
    char *client_ip;
    int n;
    int result;
    struct hostent *client_name; /* client host info */

    while(1)
    {
    	client_sock = accept(sock, (struct sockaddr *) &client, &client_len);
    	if (client_sock == -1)
    		DieWithError("accept() failed: ");

    	//HANDLE MESSAGE
    	bzero(buf, BUFFSIZE);  //ZERO OUT BUFFER
    	n = read(client_sock, buf, BUFFSIZE);
    	if (n < 0)
    		DieWithError("read() failed: ");

    	// GET CLIENT INFO
    	client_name = gethostbyaddr((const char *)&client.sin_addr.s_addr,
    				  sizeof(client.sin_addr.s_addr), AF_INET);

    	if (client_name == NULL)
    		DieWithError("host unknown");

    	client_ip = inet_ntoa(client.sin_addr);

    	if (client_ip == NULL)
    		DieWithError("host unknown");


    	log_write("server.log", client_name->h_name, client_ip, port, buf, 1);

    	result = handle_message(kv_store, buf, response);

    	n = write(client_sock, response, strlen(response));
    	if (n < 0)
    		DieWithError("write failed: ");

    	log_write("server.log", client_name->h_name, client_ip, port, response, 0);

    	close(client_sock);

    }

    return sock;
}


int handle_message(kv* kv_store, char* msg, char* response)
{
	int command = 0;
	int key = 0;
	int value = 0;
	int r = kv_parser(msg, &command, &key, &value);

	if(r < 0)
	{
		sprintf(response, "Invalid Message %d: %s", r, msg);
		return (-1);  //MALFORMED
	}

	int s = 0;
	switch(command)
	{
	case 0 :
		s = kv_put(kv_store, key, value);
		if (s == 0)
			sprintf(response, "Key Stored Successfully");
		else
			sprintf(response, "Error storing value %d into key %d.", value, key);
		break;
	case 1: //GET
		s = kv_get(kv_store, key, &value);
		if (s == 0)
			sprintf(response, "%d", value);
		else
			sprintf(response, "Value not found");
		break;
	case 2: //DEL
		s = kv_del(kv_store, key);
		if (s == 0)
			sprintf(response, "Deleted Successfully.");
		else
			sprintf(response, "Error Removing Value");
		break;

	default:
		sprintf(response, "Unable to interpret command");
		return (-1); //BAD COMMAND
		break;
	}

	return(0);
}
