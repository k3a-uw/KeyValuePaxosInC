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
char** servers;
char myname[1024];
int server_count;

/********************************************************
 * RPC FUNCTION FOR RESPONDING TO RPC CALLS FOR GETTING *
 * A VALUE FROM THE KEY VALUE STORE.  INDATA IS THE     *
 * MESSAGE PROVIDED BY THE RPC CALLER.                  *
 *******************************************************/
xdrMsg * server_rpc_get(xdrMsg * indata) {
	char s_command[BUFFSIZE];
	sprintf(s_command, "GET=%d", indata->key);
	log_write("server.log", "unknown (RPC)", "(unknown RPC)", 0, s_command, 1);
	xdrMsg outdata;
	int value;
	int result = kv_get(kv_store, indata->key, &value);

	if (result == 0) {
		outdata.key = 0;
		outdata.value = value;
	} else {
		outdata.key = -1;
		outdata.value = 0;
	}
	sprintf(s_command, "Key=%d,Value=%d", outdata.key, outdata.value);
	log_write("server.log", "unknown (RPC)", "unknown (RPC)", 0, s_command, 0);
	return (&outdata);

}

/********************************************************
 * RPC FUNCTION FOR RESPONDING TO RPC CALLS FOR PUTTING *
 * A VALUE INTO THE KEY VALUE STORE.  INDATA IS THE     *
 * MESSAGE PROVIDED BY THE RPC CALLER.                  *
 *******************************************************/
xdrMsg * server_rpc_put(xdrMsg * indata) {

	// APPLY LOCK TO OUR KEY
	kv_set_lock_status(kv_store, indata->key, KV_LOCKED);

	// SEND PREPARE TO COMMIT TO ALL SERVERS  (WHEN A SERVER RECEIVED A 'PREPARE TO COMMIT, THEY APPLY A LOCK)
	xdrMsg message = { 0 };
	xdrMsg response = { 0 };
	xdrMsg outdata = { 0 };
	int result;

	message.key    = indata->key;
	message.value  = indata->value;
	message.status = PREPARE;

	// tell all servers to prepare! (this will be a loop)
	int responses = 0;
	for (int i = 0; i < server_count; i++) {
		if (strcmp(myname, servers[i]) != 0) {
			int status = callrpc(servers[i],
						RPC_PROG_NUM,
						RPC_PROC_VER,
						RPC_2PC,
						xdr_rpc,
						&message,
						xdr_rpc,
						&response);
			if (status >= 0 && response.status == READY)
				responses++;
		}
	}

	// ONLY CONTINUE IF YOU HAVE ENOUGH READYS, OTHERWISE ABORT
	if (responses >= server_count - 1) {
		message.key    = indata->key;
		message.value  = indata->value;
		message.status = COMMIT_PUT;

		result = kv_put(kv_store, indata->key, indata->value);

		if (result == 0) {
			outdata.key   = 0;
			outdata.value = result;
		} else {
			outdata.key   = 0;
			outdata.value = 0;
		}

	} else {   //abort
		message.key;
		message.value;
		message.status = ABORT;
	}

	// SEND COMMIT (OR ABORT TO ALL SERVERS)
	responses = 0;
	for (int i = 0; i < server_count; i++) {
		if (strcmp(myname, servers[i]) != 0) {
			int status = callrpc(servers[i],
							RPC_PROG_NUM,
							RPC_PROC_VER,
							RPC_2PC,
							xdr_rpc,
							&message,
							xdr_rpc,
							&response);

			if (status >= 0 && response.status == OK)
				responses++;
			else
				printf("COMMIT RESPONSE FAILED");
		}
	}

	//WHEN ALL OKAYS ARE RETURNED, REMOVE LOCK
	kv_set_lock_status(kv_store, indata->key, KV_UNLOCKED);

	// RESPOND TO CLIENT AND WRITE TO THE LOCK
	char s_command[BUFFSIZE];
	sprintf(s_command, "PUT Key=%d Value=%d", indata->key, indata->value);
	log_write("server.log", "unknown (RPC)", "(unknown RPC)", 0, s_command, 1);

	sprintf(s_command, "Key=%d,Value=%d", outdata.key, outdata.value);
	log_write("server.log", "unknown (RPC)", "unknown (RPC)", 0, s_command, 0);

	return (&outdata);

}

xdrMsg * server_rpc_2pc(xdrMsg * indata) {
	printf("Did I get to the 2PC command!?\n");

	xdrMsg outdata = { 0 };
	switch (indata->status) {
	case PREPARE:
		if (kv_get_lock_status(kv_store, indata->key == KV_LOCKED)) {
			outdata.status = NACK;
		} else {
			kv_set_lock_status(kv_store, indata->key, KV_LOCKED);
			outdata.status = READY;
		}
		break;

	case COMMIT_PUT:
		kv_put(kv_store, indata->key, indata->value);
		kv_set_lock_status(kv_store, indata->key, KV_UNLOCKED);
		outdata.status = OK;
		break;

	case COMMIT_DEL:
		kv_del(kv_store, indata->key);
		outdata.status = OK;
		break;

	case ABORT:
		kv_set_lock_status(kv_store, indata->key, KV_UNLOCKED);
		outdata.status = OK;
		break;

	default:
		outdata.status = NACK;
		break;
	}

	return (&outdata);

}

/*********************************************************
 * RPC FUNCTION FOR RESPONDING TO RPC CALLS FOR DELETING *
 * A VALUE FROM THE KEY VALUE STORE.  INDATA IS THE      *
 * MESSAGE PROVIDED BY THE RPC CALLER.                   *
 *******************************************************/
xdrMsg * server_rpc_del(xdrMsg * indata) {

	// APPLY LOCK TO OUR KEY
	// SEND PREPARE TO COMMIT TO ALL SERVERS  (WHEN A SERVER RECEIVED A 'PREPARE TO COMMIT, THEY APPLY A LOCK)
	// WAIT FOR RESPONSE FROM ALL SERVERS
	// IF NACK IS RECEIVED, THEN ABORT (SEND MESSAGE TO CLIENT SAYING "FAIL!")
	// IF ALL RESPONSES ARE YES
	// APPLY CHANGE LOCALLY
	// SEND "COMMIT"  (SPIN UP 4 THREADS AND CALL RPC_COMMIT)
	// WAIT FOR "OK" AND RECORD OKAY TO THE LOG.
	// RETURN WHEN ALL OK ARE RECEIVED.
	// RELEASE LOCK.
	char s_command[BUFFSIZE];
	sprintf(s_command, "DEL=%d", indata->key);
	log_write("server.log", "unknown (RPC)", "(unknown RPC)", 0, s_command, 1);

	xdrMsg outdata;
	int result = kv_del(kv_store, indata->key);

	if (result == 0) {
		outdata.key = 0;
		outdata.value = result;
	} else {
		outdata.key = -1;
		outdata.value = result;
	}

	sprintf(s_command, "Key=%d,Value=%d", outdata.key, outdata.value);
	log_write("server.log", "unknown (RPC)", "unknown (RPC)", 0, s_command, 0);

	return (&outdata);
}

/********************************************************
 * THE FUNCTION CALLED BY THE MAIN MENU THAT SETSUP THE *
 * RPC FUNCTIONS AND BEGINS TO LISTEN FOR INCOMING      *
 * MEESAGES. DOES NOT RETURN.                           *
 *******************************************************/
int server_rpc_init(char** servers_list, int the_server_count) {
	server_count = the_server_count;
	servers = servers_list;
	struct utsname unameData;
	uname(&unameData);

	strcpy(myname, unameData.nodename);
	printf("The host name is: %s\n", myname);

	// INITIALIZE DATA STRUCTURES.
	int status;
	kv_store = kv_new();

	for (int i = 0; i < server_count; i++)
		printf("Loaded Server: %s\n", servers[i]);

	printf("Server Load Complete...\n");

	printf("Registering RPC...\n");

	status = registerrpc(RPC_PROG_NUM, RPC_PROC_VER, RPC_PUT, server_rpc_put,
			xdr_rpc, &xdr_rpc);

	if (status < 0)
		printf("PUT FAILED TO REGISTER\n");

	status = registerrpc(RPC_PROG_NUM, RPC_PROC_VER, RPC_GET, server_rpc_get,
			xdr_rpc, &xdr_rpc);

	if (status < 0)
		printf("GET FAILED TO REGISTER\n");

	status = registerrpc(RPC_PROG_NUM, RPC_PROC_VER, RPC_DEL, server_rpc_del,
			xdr_rpc, &xdr_rpc);

	if (status < 0)
		printf("DEL FAILED TO REGISTER\n");

	status = registerrpc(RPC_PROG_NUM, RPC_PROC_VER, RPC_2PC, server_rpc_2pc,
			xdr_rpc, &xdr_rpc);

	if (status < 0)
		printf("2PC FAILED TO REGISTER\n");

	printf("Now Listening for Commands...\n");
	svc_run();

	printf("Exiting due to svc_run failure....\n"); // svc_runs forever.

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
int server_tcp_init(unsigned short port_num) {
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

	while (1) {
		client_sock = accept(sock, (struct sockaddr *) &client, &client_len);
		if (client_sock == -1)
			ServerErrorHandle("accept() failed: ");

		// GET CLIENT INFO
		client_name = gethostbyaddr((const char *) &client.sin_addr.s_addr,
				sizeof(client.sin_addr.s_addr), AF_INET);

		if (client_name == NULL || client_ip == NULL)
			ServerErrorHandle("host unknown");

		client_ip = inet_ntoa(client.sin_addr);

		while (1) {  //HANDLE INCOMING MESSAGE
			bzero(buf, BUFFSIZE);  //ZERO OUT BUFFER
			n = recv(client_sock, buf, BUFFSIZE, MSG_WAITALL);
			if (n < 0)
				ServerErrorHandle("read() failed: ");
			else if (n == 0) //CONNECTION CLOSED
				break;

			if (strlen(buf) > 1) {
				log_write("server.log", client_name->h_name, client_ip,
						port_num, buf, 1);
				bzero(response, BUFFSIZE);
				result = server_handle_message(buf, response);
				n = send(client_sock, response, BUFFSIZE, 0);
				if (n < 0)
					printf("Responding Failed\n"); //ServerErrorHandle("write failed: ");
				log_write("server.log", client_name->h_name, client_ip,
						port_num, response, 0);
			}
		} //CLOSE INNER WHILE
		close(client_sock);

	}

	return (0);
}

/******************************************
 * LAUNCHES A UDP SERVER TO LISTEN ON THE *
 * PORT PROVIDED.  RUNS UNTIL PROCES IS   *
 * KILLED.  WILL FAIL IF SOCKETS ARE      *
 * UNABLE TO BE CREATED, BOUND, ETC.      *
 *****************************************/
int server_udp_init(unsigned short port_num) {
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

	while (1) {
		bzero(buf, BUFFSIZE);  //ZERO OUT BUFFER
		n = recvfrom(sock, buf, BUFFSIZE, 0, (struct sockaddr*) &client,
				&client_len);
		if (n < 0)
			ServerErrorHandle("recvfrom() failed");

		client_name = gethostbyaddr((const char *) &client.sin_addr.s_addr,
				sizeof(client.sin_addr.s_addr), AF_INET);

		client_ip = inet_ntoa(client.sin_addr);
		if (client_name == NULL || client_ip == NULL)
			ServerErrorHandle("host unknown");

		if (strlen(buf) > 1) {
			// WRITE THE MESSAGE RECEIVED TO THE SERVER LOG
			log_write("server.log", client_name->h_name, client_ip, port_num,
					buf, 1);

			// PROCESS THE MESSAGE
			bzero(response, BUFFSIZE);
			int result = server_handle_message(buf, response);

			// RESPOND WITH RESULTS
			n = sendto(sock, response, BUFFSIZE, 0, (struct sockaddr *) &client,
					client_len);
			log_write("server.log", client_name->h_name, client_ip, port_num,
					response, 0);

			if (n < 0)
				printf("Responding Failed\n"); //ServerErrorHandle("write failed: ");
		}
	}
	close(sock);
	return (0);
}

/******************************************
 * INTERPRETS THE MESSAGE RECEIVED BY THE *
 * SERVER FROM THE CLIENT AND DETERMINES  *
 * WHAT ACTION TO TAKE.  IF THE MESSAGE IS*
 * MALFORMED IT WILL RETURN -1 OTHERWISE  *
 * THE RESPONSE WILL BE STORED IN THE     *
 * BUFFER PROVIDED.                       *
 *****************************************/
int server_handle_message(char* msg, char* response) {
	int command = 0;
	int key = 0;
	int value = 0;
	int r = kv_parser(msg, &command, &key, &value);

	if (r < 0) {
		sprintf(response, "Invalid Message %d: %s", r, msg);
		return (-1);  //MALFORMED
	}

	int s = 0;
	switch (command) {
	case 0:
		s = kv_put(kv_store, key, value);
		if (s == 0)
			sprintf(response, "Key Stored Successfully");
		else
			sprintf(response, "Error storing value %d into key %d.", value,
					key);
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

	return (0);
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
