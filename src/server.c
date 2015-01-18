
#ifndef SERVER_H
#include "server.h"
#endif


int server_tcp_init(unsigned short port_num)
{
	printf("You are running TCP Server on port number: %d.", port_num);

	// CREATE A NEW KEY VALUE STORE
	kv* kv_store = kv_new();

    struct sockaddr_in server;

    /* Create socket for incoming connections */
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    // USE SOCK_DGRAM INSTEAD OF SOCK_STREAM
    if (sock < 0)
        ServerErrorHandle("socket() failed: ");

    /* Construct local address structure */
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port_num);
    server.sin_addr.s_addr = INADDR_ANY;

    /* Bind to the local address */
    int bindsock = bind(sock, (struct sockaddr *) &server, sizeof(server));
    if (bindsock < 0)
    	ServerErrorHandle("bind() failed: ");

    /* Listen to the incoming socket */
    int listensock = listen(sock, MAXPENDING);
    if (listensock < 0)
    	ServerErrorHandle("listen() failed: ");

    int client_sock;
    struct sockaddr_in client;
    int client_len = sizeof(client);

    char buf[BUFFSIZE]; /* message buffer */
    char response[BUFFSIZE];
    char *client_ip;
    int n;
    int result;
    struct hostent *client_name; /* client host info */

    while(1)
    {
    	client_sock = accept(sock, (struct sockaddr *) &client, &client_len);
    	if (client_sock == -1)
    		ServerErrorHandle("accept() failed: ");
    	else

    	//HANDLE MESSAGE
    	bzero(buf, BUFFSIZE);  //ZERO OUT BUFFER
    	printf("Waiting for input...");
    	n = read(client_sock, buf, BUFFSIZE);
    	if (n < 0)
    		ServerErrorHandle("read() failed: ");

    	// GET CLIENT INFO
    	client_name = gethostbyaddr((const char *)&client.sin_addr.s_addr,
    				  sizeof(client.sin_addr.s_addr), AF_INET);

       	if (client_name == NULL || client_ip == NULL)
        		ServerErrorHandle("host unknown");

    	client_ip = inet_ntoa(client.sin_addr);

    	log_write("server.log", client_name->h_name, client_ip, port_num, buf, 1);

    	result = server_handle_message(kv_store, buf, response);

    	n = write(client_sock, response, strlen(response));
    	if (n < 0)
    		ServerErrorHandle("write failed: ");

    	log_write("server.log", client_name->h_name, client_ip, port_num, response, 0);

    	close(client_sock);

    }

    return(0);
}

int server_udp_init(unsigned short port_num)
{
	printf("You are running UDP Server on port number: %d.", port_num);

	// CREATE A NEW KEY VALUE STORE
	kv* kv_store = kv_new();

    struct sockaddr_in server;

    /* Create socket for incoming connections */
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    // USE SOCK_DGRAM INSTEAD OF SOCK_STREAM
    if (sock < 0)
        ServerErrorHandle("socket() failed: ");

    /* Construct local address structure */
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port_num);
    server.sin_addr.s_addr = INADDR_ANY;

    /* Bind to the local address */
    int bindsock = bind(sock, (struct sockaddr *) &server, sizeof(server));
    if (bindsock < 0)
    	ServerErrorHandle("bind() failed: ");

    struct sockaddr_in client;
    int client_len = sizeof(client);

    char buf[BUFFSIZE]; /* message buffer */
    char response[BUFFSIZE];
    char *client_ip;
    int n;
    int m;
    int result;
    struct hostent *client_name; /* client host info */

    while(1)
    {
    	//HANDLE MESSAGE
    	bzero(buf, BUFFSIZE);  //ZERO OUT BUFFER
    	printf("Waiting for input...");
    	n = recvfrom(sock, buf, BUFFSIZE, 0, (struct sockaddr*) &client, &client_len);
    	if (n < 0)
    		ServerErrorHandle("recvfrom failed: ");

    	// GET CLIENT INFO
    	client_name = gethostbyaddr((const char *)&client.sin_addr.s_addr,
    				  sizeof(client.sin_addr.s_addr), AF_INET);

    	if (client_name == NULL || client_ip == NULL)
    		ServerErrorHandle("host unknown");

    	client_ip = inet_ntoa(client.sin_addr);

    	log_write("server.log", client_name->h_name, client_ip, port_num, buf, 1);

    	bzero(response, BUFFSIZE);
    	result = server_handle_message(kv_store, buf, response);

		m = sendto(sock,response, strlen(response), 0, (struct sockaddr *) &client, client_len);
    	if (m < 0)
    		ServerErrorHandle("write failed!!!");

    	log_write("server.log", client_name->h_name, client_ip, port_num, response, 0);


    }
	close(sock);

    return(0);
}



int server_handle_message(kv* kv_store, char* msg, char* response)
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

void ServerErrorHandle(char *errorMessage) /* Error handling function */
{
	perror(errorMessage);
	exit(-1);
}
