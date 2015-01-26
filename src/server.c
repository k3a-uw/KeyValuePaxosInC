/*
 ============================================================================
 Name        : server.c
 Author      : Kevin Anderson <k3a@uw.edu> & Daniel Kristiyanto <danielkr@uw.edu>
 Version     : 2015.01.18
 Description : Listens and handles incoming messages from a client.  It will
             : add, put and delete values into a key value store.  Runs
             : forever until a kill or ctrl+c command is ran.
 ============================================================================
 */

#ifndef SERVER_H
#include "server.h"
#endif

kv* kv_store;
messagequeue* mq;  // monitored by agents, populated by server
messagequeue* sq;  // populated by agents, monitored by server

char* server_rpc_proc(char * message)
{
	printf("The message was! %s\n", message);
	char * to_return;

	strcpy(to_return, message);
	return &to_return;
}


int server_rpc_init(unsigned short port_num)
{

	// INITIALIZE DATA STRUCTURES.
	kv_store = kv_new();
	mq = mq_new();
	sq = mq_new();

	// STARTUP THE THREADS
	pthread_t thread[THREAD_COUNT];
	for(int i = 0; i < THREAD_COUNT ; i++)
	{
		pthread_create(&thread[i], NULL, MessageAgent, i);
	}

	printf("Registering RPC...");
	registerrpc(1234,123,12, server_rpc_proc, xdr_string, xdr_string);

	printf("Running the RPC_Service");
	svc_run();

	printf("Exiting due to svc_run failure.\n"); // svc_runs forever.

		//ON MESSAGE, ADD IT TO QUEUE
		//mq_push(mq, message, client);


		//READ FORM THE MESSAGE QUEUE AND SEND RESPONSES TO CLIENT
		//if (mq_pull(sq, response, res_client) >= 0)  {  //SEND A RESPONSE TO THE CLIENT }
		//server_rpc_respond(response, res_client);

}



/******************************************
 * LAUNCHES A TCP SERVER TO LISTEN ON THE *
 * PORT PROVIDED.  RUNS UNTIL PROCES IS   *
 * KILLED.  WILL FAIL IF SOCKETS ARE      *
 * UNABLE TO BE CREATED, BOUND, ETC.      *
 *****************************************/
int server_tcp_init(unsigned short port_num)
{
	printf("You are running TCP Server on port number: %d.\n", port_num);

	// CREATE A NEW KEY VALUE STORE

	kv_store = kv_new();

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

	printf("Waiting for input...\n");

	while(1)
    {
		client_sock = accept(sock, (struct sockaddr *) &client, &client_len);
		if (client_sock == -1)
			ServerErrorHandle("accept() failed: ");


		// GET CLIENT INFO
		client_name = gethostbyaddr((const char *)&client.sin_addr.s_addr,
					  sizeof(client.sin_addr.s_addr), AF_INET);

	   	if (client_name == NULL || client_ip == NULL)
	    		ServerErrorHandle("host unknown");

		client_ip = inet_ntoa(client.sin_addr);

		while(1) {  //HANDLE INCOMING MESSAGE
			bzero(buf, BUFFSIZE);  //ZERO OUT BUFFER
			n = recv(client_sock, buf, BUFFSIZE, MSG_WAITALL);
			if (n < 0)
				ServerErrorHandle("read() failed: ");
			else if (n == 0) //CONNECTION CLOSED
				break;

			if (strlen(buf) > 1)
			{
				log_write("server.log", client_name->h_name, client_ip, port_num, buf, 1);
				bzero(response, BUFFSIZE);
				result = server_handle_message(buf, response);
				n = send(client_sock, response, BUFFSIZE,0);
				if (n < 0)
					printf("Responding Failed\n"); //ServerErrorHandle("write failed: ");
				log_write("server.log", client_name->h_name, client_ip, port_num, response, 0);
			}
		} //CLOSE INNER WHILE
		close(client_sock);

    }

    return(0);
}

/******************************************
 * LAUNCHES A UDP SERVER TO LISTEN ON THE *
 * PORT PROVIDED.  RUNS UNTIL PROCES IS   *
 * KILLED.  WILL FAIL IF SOCKETS ARE      *
 * UNABLE TO BE CREATED, BOUND, ETC.      *
 *****************************************/
int server_udp_init(unsigned short port_num)
{
	printf("You are running UDP Server on port number: %d.\n", port_num);

	// CREATE A NEW KEY VALUE STORE
	kv_store = kv_new();

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
    struct hostent *client_name; /* client host info */

	printf("Waiting for input...\n");

	while(1)
    {
		bzero(buf, BUFFSIZE);  //ZERO OUT BUFFER
		n = recvfrom(sock, buf, BUFFSIZE, 0, (struct sockaddr*) &client, &client_len);
		if (n < 0)
			ServerErrorHandle("recvfrom() failed");

		client_name = gethostbyaddr((const char *)&client.sin_addr.s_addr,
					  sizeof(client.sin_addr.s_addr), AF_INET);

		client_ip = inet_ntoa(client.sin_addr);
		if (client_name == NULL || client_ip == NULL)
			ServerErrorHandle("host unknown");

		if (strlen(buf) > 1)
		{
			// WRITE THE MESSAGE RECEIVED TO THE SERVER LOG
			log_write("server.log", client_name->h_name, client_ip, port_num, buf, 1);

			// PROCESS THE MESSAGE
			bzero(response, BUFFSIZE);
			int result = server_handle_message(buf, response);

			// RESPOND WITH RESULTS
			n = sendto(sock, response, BUFFSIZE, 0, (struct sockaddr *) &client, client_len);
			log_write("server.log", client_name->h_name, client_ip, port_num, response, 0);

			if (n < 0)
				printf("Responding Failed\n"); //ServerErrorHandle("write failed: ");
		}
    }
	close(sock);
    return(0);
}


/******************************************
 * INTERPRETS THE MESSAGE RECEIVED BY THE *
 * SERVER FROM THE CLIENT AND DETERMINES  *
 * WHAT ACTION TO TAKE.  IF THE MESSAGE IS*
 * MALFORMED IT WILL RETURN -1 OTHERWISE  *
 * THE RESPONSE WILL BE STORED IN THE     *
 * BUFFER PROVIDED.                       *
 *****************************************/
int server_handle_message(char* msg, char* response)
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

/**********************************
 * WRITES AN ERROR TO THE CONSOLE *
 * AND EXIST THE PROGRAM          *
 *********************************/
void ServerErrorHandle(char *errorMessage) /* Error handling function */
{
	perror(errorMessage);
	exit(-1);
}


void server_read_and_respond(char* message, char* client)
{
	char response[BUFFSIZE];
	bzero(response, BUFFSIZE);
	int result = server_handle_message(message, response);

	if (result >= 0)  // WE HAVE A SUCCESSFUL RESULT SO RESPOND TO THE CLIENT
		server_respond(response, client);
}

void server_respond(char* message, char* client)
{
	//TODO SEND RESPONSE CODE

}


void *MessageAgent(void* args)
{
	char message[128] = "";
	char client[128]  = "";
	char response[128] = "";
	int result;
	printf("%d is Has Started!\n", (int) args);

	// SIMPLE WHILE LOOP. POLL QUEUE UNTIL SOMETHING ARRIVES.
	while(1)
	{
//		printf("%d is running!\n", (int) args);
		if(mq_pull(mq, message, client) >= 0)  // WHEN SOMETHING ARRIVES, HANDLE IT.
		{
			result = server_handle_message(message, response);  //PARSE THE MESSAGE AND GENERATE A RESPONSE MESSAGE
			mq_push(sq, response, client);  // ADD IT TO THE RESPONSE QUEUE
		}

	}

}

