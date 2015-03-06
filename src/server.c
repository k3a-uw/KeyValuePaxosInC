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
int quarom_count;

int my_lc  = -1;  // my lamport clock (for proposals)
int hpc    = -1;  //my highest promised clock
xdrMsg hpv = { 0 };  // my highest proposed value

xdrMsg outdata_get     = { 0 };
xdrMsg outdata_propose = { 0 };
xdrMsg outdata_del     = { 0 };
xdrMsg outdata_learn   = { 0 };
xdrMsg outdata_prepare = { 0 };
xdrMsg outdata_accept  = { 0 };

struct timespec fail;

///********************************************************
// * RPC FUNCTION FOR RESPONDING TO RPC CALLS FOR GETTING *
// * A VALUE FROM THE KEY VALUE STORE.  INDATA IS THE     *
// * MESSAGE PROVIDED BY THE RPC CALLER.                  *
// *******************************************************/
//
//// PROPOSER CODE
//xdrMsg * server_rpc_get(xdrMsg * indata) {
//	char s_command[BUFFSIZE];
//	sprintf(s_command, "RECV=GET(%d)", indata->key);
//	log_write("server.log", myname, s_command);
//	xdrMsg* outdata = (xdrMsg *) malloc(sizeof(outdata));
//	int value;
//	int result = kv_get(kv_store, indata->key, &value);
//
//	if (result == 0) {
//		sprintf(s_command, "SENT=VALUE(%d)", value);
//		outdata->key    = 0;
//		outdata->value  = value;
//		outdata->status = OK;
//	} else {
//		sprintf(s_command, "SENT=KEYNOTFOUND(%d)", indata->key);
//		outdata->key    = -1;
//		outdata->value  = 0;
//		outdata->status = OK;
//	}
//
//
//
//	xdr_free(xdr_rpc, outdata);
//
//	log_write("server.log", myname, s_command);
//	return (outdata);
//
//}

/********************************************************
 * RPC FUNCTION FOR RESPONDING TO RPC CALLS FOR PUTTING *
 * A VALUE INTO THE KEY VALUE STORE.  INDATA IS THE     *
 * MESSAGE PROVIDED BY THE RPC CALLER.                  *
 *******************************************************/
// PROPOSER CODE
////xdrMsg * server_rpc_put(xdrMsg * indata) {
////
////	char s_command[BUFFSIZE];
////	char r_command[BUFFSIZE];
////	sprintf(s_command, "RECV=PUT(%d,%d)", indata->key, indata->value);
////	log_write("server.log", "client", s_command);
////
////	// APPLY LOCK TO OUR KEY
////	kv_set_lock_status(kv_store, indata->key, KV_LOCKED);
////
////	// SEND PREPARE TO COMMIT TO ALL SERVERS  (WHEN A SERVER RECEIVED A 'PREPARE TO COMMIT, THEY APPLY A LOCK)
////	xdrMsg message  = { 0 };
////	xdrMsg response = { 0 };
////
////	int result;
////
////	message.key    = indata->key;
////	message.value  = indata->value;
////	message.status = PREPARE;
////
////	// tell all servers to prepare! (this will be a loop)
////	int responses = 0;
////	for (int i = 0; i < server_count; i++) {
//////		printf("Sending Prepare to %s.\n", servers[i]);
////		if (strcmp(myname, servers[i]) != 0) {
////			sprintf(s_command, "SENT=PREPARE(%d)", message.key);
////			log_write("server.log", servers[i], s_command);
////			int status = callrpc(servers[i],
////						RPC_PROG_NUM,
////						RPC_PROC_VER,
////						RPC_2PC,
////						xdr_rpc,
////						&message,
////						xdr_rpc,
////						&response);
////			if (status != 0)
////			{
////				sprintf(s_command, "RECV=SEND FAILURE");
////			} else if(response.status == READY) {
////				responses++;
////				sprintf(s_command, "RECV=READY(%d)", message.key);
////			} else {
////				sprintf(s_command, "RECV=NACK(%d)", message.key);
////			}
////
////			log_write("server.log", servers[i], s_command);
////		}
////	}
////
////
////	int out_key;
////	int out_value;
////	int out_status;
////
////	// ONLY CONTINUE IF YOU HAVE ENOUGH READYS, OTHERWISE ABORT
////	if (responses >= server_count - 1) {
////		message.key    = indata->key;
////		message.value  = indata->value;
////		message.status = COMMIT_PUT;
////		sprintf(s_command, "SENT=COMMIT_PUT(%d,%d)", message.key);
////
////		//printf("Enough Responses, so I'm sending performing my change!!\n");
////
////		result = kv_put(kv_store, indata->key, indata->value);
////
////		if (result == 0) {
//////			printf("The put of key %d was successful.\n", indata->key);
////			out_key    = 0;
////			out_value  = result;
////			out_status = OK;
////			sprintf(r_command, "SENT=PUT_SUCCESS(%d,%d)", indata->key, out_value);
////		} else {
//////			printf("The put of key %d was NOT successful.\n", indata->key);
////			out_key    = -1;
////			out_value  = 0;
////			out_status = OK;
////			sprintf(r_command, "SENT=PUT_FAILURE(%d,%d)", indata->key, indata->value);
////		}
////
////	} else {   //abort
//////		printf("I didn't get enough responses, so I'm aborting\n");
////		message.key    = indata->key;
////		message.value  = -1;
////		message.status = ABORT;
////		out_key   = -1;
////		out_value = -1;
////		out_status = ABORT;
////		sprintf(s_command, "SENT=ABORT(%d)", message.key);  // MESSAGE TO SENT TO SERVERS
////		sprintf(r_command, "SENT=PUT_FAILURE(%d)", message.key);
////	}
////
////	// SEND COMMIT (OR ABORT TO ALL SERVERS)
////	responses = 0;
////	for (int i = 0; i < server_count; i++) {
////		if (strcmp(myname, servers[i]) != 0) {
////			log_write("server.log", servers[i], s_command);
////			int status = callrpc(servers[i],
////							RPC_PROG_NUM,
////							RPC_PROC_VER,
////							RPC_2PC,
////							xdr_rpc,
////							&message,
////							xdr_rpc,
////							&response);
////			if (status != 0) {
////				sprintf(s_command, "RECV=SEND FAILURE");
////			} else if (response.status == OK) {
////				responses++;
////				sprintf(s_command, "RECV=OK");
////			} else {
////				sprintf(s_command, "RECV=NACK");
////			}
////			log_write("server.log", servers[i], s_command);
////		}
////	}
////
////	//WHEN ALL OKAYS ARE RETURNED, REMOVE LOCK
////	kv_set_lock_status(kv_store, indata->key, KV_UNLOCKED);
////
////
////	xdrMsg * outdata = (xdrMsg *) malloc(sizeof(outdata));
////
////	outdata->key    = out_key;
////	outdata->value  = out_value;
////	outdata->status = out_status;
////
////
////	// RESPOND TO CLIENT AND WRITE TO THE LOG
////	log_write("server.log", "client", r_command);
////
////	// FREE THE DATA FOR NEXT TIME;
////	xdr_free(xdr_rpc, outdata);
////	return (outdata);
////
////}
//
//// PROPOSER CODE
//xdrMsg * server_rpc_del(xdrMsg * indata) {
//
//	// APPLY LOCK TO OUR KEY
//	char s_command[BUFFSIZE];
//	char r_command[BUFFSIZE];
//	sprintf(s_command, "RECV=DEL(%d)", indata->key);
//	log_write("server.log", "client", s_command);
//
//	// SEND PREPARE TO COMMIT TO ALL SERVERS  (WHEN A SERVER RECEIVED A 'PREPARE TO COMMIT, THEY APPLY A LOCK)
//	xdrMsg message    = { 0 };
//	xdrMsg response   = { 0 };
//
//	xdrMsg * outdata = (xdrMsg *) malloc(sizeof(outdata));
//
//	// ONLY MOVE FORWRD IF KEY EXISTS
//	if (kv_exists(kv_store, indata->key) < 0)
//	{
//		outdata->key = -1;
//		outdata->value = -1;
//		outdata->status = NACK;
//		sprintf(s_command, "SENT=KEYNOTFOUND(%d)", indata->key);
//		log_write("server.log", "client", s_command);
//		return(outdata);
//	}
//
//	kv_set_lock_status(kv_store, indata->key, KV_LOCKED);
//
//	int result;
//
//	// BUILD THE PREPARE MESSAGE
//	message.key    = indata->key;
//	message.value  = indata->value;
//	message.status = PREPARE;
//
//	// TELL ALL SERVERS TO PREPARE
//	int responses = 0;
//	for (int i = 0; i < server_count; i++) {
//		if (strcmp(myname, servers[i]) != 0) {
//			sprintf(s_command, "SENT=PREPARE(%d)", message.key);
//			log_write("server.log", servers[i], s_command);
//			int status = callrpc(servers[i],
//						RPC_PROG_NUM,
//						RPC_PROC_VER,
//						RPC_2PC,
//						xdr_rpc,
//						&message,
//						xdr_rpc,
//						&response);
//			if (status != 0)
//			{
//				sprintf(s_command, "RECV=SEND FAILURE");
//			} else if (response.status == READY) {
//				responses++;
//				sprintf(s_command, "RECV=READY(%d)", message.key);
//			} else {
//				sprintf(s_command, "RECV=NACK(%d)", message.key);
//			}
//		}
//	}
//
////	printf("Received %d of the required %d READY responses.\n", responses, server_count-1);
//
//	// ONLY CONTINUE IF YOU HAVE ENOUGH READYS, OTHERWISE ABORT
//	if (responses >= server_count - 1) {
//		message.key    = indata->key;
//		message.value  = indata->value;
//		message.status = COMMIT_DEL;
//		sprintf(r_command, "DEL_SUCCESS(%d)", indata->key); //  message for client
//		sprintf(s_command, "COMMIT_DEL(%d)", indata->key);  // message for servers
//
////		printf("Enough Responses, so I'm sending performing my change!!\n");
//
//		result = kv_del(kv_store, indata->key);
//
//		if (result == 0) {
////			printf("The delete of key %d was successful.\n", indata->key);
//			outdata->key   = 0;
//			outdata->value = result;
//			outdata->status = OK;
//		} else {
////			printf("The delete of key %d was NOT successful.\n", indata->key);
//			outdata->key   = -1;
//			outdata->value = 0;
//			outdata->status = OK;
//		}
//
//	} else {   //abort
//		sprintf(r_command, "DEL_FAILURE(%d)", indata->key);  // message for the client;
//		sprintf(s_command, "ABORT(%d)", indata->key);
////		printf("I didn't get enough responses, so I'm aborting\n");
//		message.key    = -1;
//		message.value  = -1;
//		message.status = ABORT;
//	}
//
//	// SEND COMMIT (OR ABORT TO ALL SERVERS)
//	responses = 0;
////	printf("Sending Status: %d to all servers! (Positive is commit, negative is abort!\n", message.status);
//	for (int i = 0; i < server_count; i++) {
//		if (strcmp(myname, servers[i]) != 0) {
//			log_write("server.log",servers[i], s_command);
//			int status = callrpc(servers[i],
//							RPC_PROG_NUM,
//							RPC_PROC_VER,
//							RPC_2PC,
//							xdr_rpc,
//							&message,
//							xdr_rpc,
//							&response);
//
//			if (status != 0)
//			{
//				sprintf(s_command, "RECV=SEND FAILURE");
//			} else if (response.status == OK) {
//				responses++;
//				sprintf(s_command, "RECV=OK");
//			} else {
//				sprintf(s_command, "RECV=NACK");
//			}
//
//			log_write("server.log", servers[i], s_command);
//		}
//	}
//
//	//WHEN ALL OKAYS ARE RETURNED, REMOVE LOCK
////	printf("Setting the lock back on key %d.\n", indata->key);
//	kv_set_lock_status(kv_store, indata->key, KV_UNLOCKED);
//
//	// RESPOND TO CLIENT AND WRITE TO THE LOCK
//	log_write("server.log", "client", r_command);
//	xdr_free(xdr_rpc, outdata);
//	return (outdata);
//}

// CODE THE ACCEPTER WILL RUN WHEN IT RECEIVES AN ACCEPT
int server_rpc_init(char** servers_list, int the_server_count) {

	srand(time(NULL));  // seed the random number generator for failures.
	fail.tv_sec = FAIL_DURATION;



	server_count = the_server_count;
	quarom_count = (server_count / 2) + 1;
	printf("With %d Servers the required servers for a quarom is %d.\n", server_count, quarom_count);
	servers = servers_list;

	my_lc = 0;


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

	status = registerrpc(RPC_PROG_NUM, RPC_PROC_VER, RPC_PUT, proposer_propose,
			xdr_rpc, &xdr_rpc);

	if (status < 0)
		printf("PUT FAILED TO REGISTER\n");

	status = registerrpc(RPC_PROG_NUM, RPC_PROC_VER, RPC_DEL, proposer_propose,
			xdr_rpc, &xdr_rpc);

	if (status < 0)
		printf("DEL FAILED TO REGISTER\n");

	status = registerrpc(RPC_PROG_NUM, RPC_PROC_VER, RPC_GET, proposer_get,
			xdr_rpc, &xdr_rpc);

	if (status < 0)
		printf("GET FAILED TO REGISTER\n");


//	status = registerrpc(RPC_PROG_NUM, RPC_PROC_VER, RPC_2PC, server_rpc_2pc,
//			xdr_rpc, &xdr_rpc);
//
//	if (status < 0)
//		printf("2PC FAILED TO REGISTER\n");

	status = registerrpc(RPC_PROG_NUM, RPC_PROC_VER, RPC_PREPARE, acceptor_prepare,
			xdr_rpc, &xdr_rpc);

	if (status < 0)
		printf("ACCEPT_PREPARE FAILED TO REGISTER\n");

	status = registerrpc(RPC_PROG_NUM, RPC_PROC_VER, RPC_ACCEPT, acceptor_accept,
			xdr_rpc, &xdr_rpc);

	if (status < 0)
		printf("ACCEPT_ACCEPT FAILED TO REGISTER\n");

	status = registerrpc(RPC_PROG_NUM, RPC_PROC_VER, RPC_LEARN, learner_learn,
			xdr_rpc, &xdr_rpc);

	if (status < 0)
		printf("LEARN FAILED TO REGISTER\n");


	printf("Now Listening for Commands...\n");
	svc_run();

	printf("Exiting due to svc_run failure....\n"); // svc_runs forever.

}



xdrMsg * acceptor_accept(xdrMsg * indata)
{
	chaos_function();


	char s_command[BUFFSIZE];

	switch (indata->command)
	{
	case RPC_PUT:
		sprintf(s_command, "RECV=ACCEPT_PUT(L=%d, K=%d, V=%d)", indata->lc, indata->key, indata->value);
		break;
	case RPC_DEL:
		sprintf(s_command, "RECV=ACCEPT_DEL(L=%d, K=%d)", indata->lc, indata->key);
		break;
	default:
		sprintf(s_command, "RECV=ACCEPT_BAD(cmd=%d, LC=%d)", indata->command, my_lc);
		log_write("server.log", "proposer", s_command);
		sprintf(s_command, "SEND=NACK");
		log_write("server.log", "proposer", s_command);
		outdata_accept = hpv;
		outdata_accept.status = NACK;
		return (&outdata_accept);
	}

	log_write("server.log", "proposer", s_command);

	if (indata->lc < hpc)
	{
		// PROVIDE THE LAST VALUE
		outdata_accept = hpv;
		outdata_accept.lc = hpc;
		outdata_accept.status = NACK;
		sprintf(s_command, "SEND=NACK(L=%d)", hpc);
	} else {
		// ACCEPT THE MESSAGE, WITH THE HIGHEST PROPOSED VALUE
		outdata_accept = hpv;
		outdata_accept.lc = hpc;
		outdata_accept.status = ACCEPT;
		if (indata->command == RPC_PUT)
		{
			sprintf(s_command, "SEND=ACCEPT_PUT(L=%d, K=%d, V=%d", outdata_accept.lc, outdata_accept.key, outdata_accept.value);
		} else {
			sprintf(s_command, "SEND=ACCEPT_DEL(L=%d, K=%d)", outdata_accept.lc, outdata_accept.key);
		}
	}

	log_write("server.log", "proposer", s_command);
	return (&outdata_accept);

}

// CODE THE ACCEPTER WILL RUN WHEN IT RECEIVES A PREPARE
xdrMsg * acceptor_prepare(xdrMsg * indata)
{
	chaos_function();

	char s_command[BUFFSIZE];

	sprintf(s_command, "RECV=PREPARE(L=%d)", indata->lc);
	log_write("server.log", "proposer", s_command);

	// WHEN ACCEPTING, IF THE REQUSTED LAMPORT LOCK IS LOWER, REJECT
	if (indata->lc < hpc)
	{
		outdata_prepare.status = NACK;
		outdata_prepare.lc = hpc;
		outdata_prepare.command = indata->command;
		outdata_prepare.key = indata->key;
		outdata_prepare.value = indata->value;
		outdata_prepare.pid = 0;
		sprintf(s_command, "SEND=NACK(L=%d)", hpc);
	} else {  // OTHERWISE, MAKE THE PROMISE AND UPDATE THE HIGHEST PROMISED VALUES.
		hpc = indata->lc; // STORE THE HPC
		hpv = *indata;  //STORE THE HPC
		outdata_prepare.status = PROMISE;
		outdata_prepare.lc = hpc;
		outdata_prepare.pid = 0;
		outdata_prepare.command = indata->command;
		outdata_prepare.key = indata->key;
		outdata_prepare.value = indata->value;
		sprintf(s_command, "SEND=PROMISE(L=%d)", hpc);
	}

	log_write("server.log", "proposer", s_command);
	return (&outdata_prepare);
}


// CODE THE LEARNER WILL RUN WHEN IT RECEIVES A LEARN FROM ACCEPTOR
xdrMsg * learner_learn(xdrMsg * indata)
{
	chaos_function();

	char s_command[BUFFSIZE];

	outdata_learn.lc = my_lc;
	outdata_learn.command = indata->command;
	outdata_learn.pid = 0;
	outdata_learn.key = indata->key;
	outdata_learn.value = indata->value;


	int result;
	switch (indata->command)
	{
	case RPC_PUT:
		sprintf(s_command, "RECV=LEARN_PUT(%d, %d, L=%d)", indata->key, indata->value, my_lc);
		log_write("server.log", "proposer", s_command);

		result = kv_put(kv_store, indata->key, indata->value);
		if (result == 0)
		{
			sprintf(s_command, "SEND=PUT_SUCCESS(%d, %d, L=%d)", indata->key, indata->value, my_lc);
			outdata_learn.status = OK;
		}
		else
		{
			sprintf(s_command, "SEND=PUT_FAILURE(%d, %d, L=%d)", indata->key, indata->value, my_lc);
			outdata_learn.status = NACK;
		}
		break;


	case RPC_DEL:
		sprintf(s_command, "RECV=LEARN_DEL(%d, L=%d)", indata->key, my_lc);
		log_write("server.log", "proposer", s_command);

		result = kv_del(kv_store, indata->key);
		if (result == 0)
		{
			sprintf(s_command, "SEND=DEL_SUCCESS(%d, L=%d)", indata->key, my_lc);
			outdata_learn.status = OK;
		} else {
			sprintf(s_command, "SEND=DEL_FAILURE(%d, L=%d)", indata->key, my_lc);
			outdata_learn.status = NACK;
		}
		break;

	case RPC_GET:

		sprintf(s_command, "RECV=LEARN_GET(%d, L=%d)", indata->key, my_lc);
		log_write("server.log", "proposer", s_command);
		int value;
		result = kv_get(kv_store, indata->key, &value);

		if (result == 0) {
			outdata_learn.status = OK;
			outdata_learn.value = value;
			sprintf(s_command, "SEND=OK(%d, L=%d)", outdata_learn.value, my_lc);
		} else {  // KEY NOT FOUND
			outdata_learn.status = NACK;
			sprintf(s_command, "SEND=NACK(L=%d", my_lc);
		}

		break;

	default:
		sprintf(s_command, "RECV=BAD_LEARN(%d, L=%d)", indata->command, my_lc);
		log_write("server.log", "proposer", s_command);
		sprintf(s_command, "SEND=NACK");
		outdata_learn.status = NACK;
		break;

	}

	log_write("server.log", "proposer", s_command);

	return(&outdata_learn);

}


// CODE THE PROPOSER WILL RUN WHEN A CLIENT SEND A GET
xdrMsg * proposer_get(xdrMsg * indata)
{

	my_lc = my_lc + 1;
	char s_command[BUFFSIZE];
	sprintf(s_command, "RECV=GET(%d, L=%d)", indata->key, my_lc);
	log_write("server.log", "client", s_command);

	xdrMsg message  = { 0 };
	xdrMsg response = { 0 };

	// SETUP THE MESSAGE TO SEND TO ALL LEARNERS.
	message.key     = indata->key;
	message.value   = -1;
	message.status  = OK;
	message.command = RPC_GET;
	message.lc      = my_lc;
	message.pid     = 0;  //TODO FIGURE OUT PROCESS IDS

	int responses[quarom_count][3];  //three columns, 0 = live value, 1 = value, 2 = count;

	// INITIALIZE THE RESPONSES ARRAY
	for (int i = 0; i < quarom_count; i++)
	{
		responses[i][0] =  -1;
		responses[i][1] =  0;
		responses[i][2] =  0;
	}

	int my_value = -1;
	// SEND LEARN_GET TO ALL LEARNERS
	int have_quarom = 0;
	int quarom_value = -1;
	for (int i = 0; i < server_count ; i++)
	{
		int response_value = 0;
		int response_status = NACK;
		int status;
		sprintf(s_command, "SEND=LEARNER_GET(%d, L=%d)", indata->key, my_lc);
		if (strcmp(myname, servers[i]) == 0)
		{  // GET THE VALUE FROM LOCAL
			log_write("server.log", "localhost", s_command);
			status = kv_get(kv_store, indata->key, &response_value);
			my_value = response_value;
			response_status = OK;
			if (status == 0)
			{
				sprintf(s_command, "RECV=OK(%d, L=%d)", response_value, my_lc);
			} else {
				sprintf(s_command, "RECV=NACK(L=%d", my_lc);
			}
			log_write("server.log", "localhost", s_command);
		} else {
			// GET THE VALUE FROM REMOTE;
			log_write("server.log", servers[i], s_command);
			status = callrpc(servers[i],
					RPC_PROG_NUM,
					RPC_PROC_VER,
					RPC_LEARN,
					xdr_rpc,
					&message,
					xdr_rpc,
					&response);

			response_status = response.status;
			response_value  = response.value;

			if (status == 0 && response_status == OK)
			{
				sprintf(s_command, "RECV=OK(%d, L=%d)", response.value, my_lc);
			} else {
				sprintf(s_command, "RECV=NACK(L=%d)", my_lc);
			}

			log_write("server.log",servers[i], s_command);

		}


		// STORE THE VALUE TO GET A QUAROM.
		if (status == 0 && response_status == OK)  // IF WE HAVE A GOOD RESULT
		{
			for(int j = 0; j < quarom_count; j++)
			{
				if (responses[j][0] == 0)  // IT IS A LIVE VALUE, CHECK IT
				{
					if (responses[j][1] == response_value) // WE HAVE A MATCH
					{
						responses[j][2] = responses[j][2] + 1;  // INCREMENT THE COUNT
						if (responses[j][2] >= quarom_count)
						{
							have_quarom = 1;
							quarom_value = responses[j][1]; // SAVE IT AS THE RETURN VALUE
							break; // END FOR LOOP
						}
					}
				} else {  // WE ARE AT AN NOT LIVE VALUE
					responses[j][0] = 0; //MAKE IT LIVE
					responses[j][1] = response_value;
					responses[j][2] = 1;
					break;   // END FOR LOOP
				}
			}
		} else {
			// REPORT THAT THE RESPONSE VALUE WAS BAD.
		}

		if (have_quarom == 1)  // if we have a quarom, we can leave.
		{
			break;
		}
	}  // LOOP TO THE NEXT SERVER

	if (have_quarom == 1)
	{
		outdata_get.key   = indata->key;
		outdata_get.value = quarom_value;
		outdata_get.status = OK;
		outdata_get.command = RPC_GET;
		outdata_get.lc = my_lc;
		outdata_get.pid = 0;
		sprintf(s_command, "SEND=OK(%d, L=%d)", outdata_get.value, my_lc);
		if (my_value != outdata_get.value)  // I'M OUT OF DATE
			kv_put(kv_store, outdata_get.key, outdata_get.value);  //SO I'M LEARNING THE VALUE
	} else {
		outdata_get.key  = indata->key;
		outdata_get.value = -1;
		outdata_get.status = NACK;
		outdata_get.command = RPC_GET;
		outdata_get.lc = my_lc;
		outdata_get.pid = 0;
		sprintf(s_command, "SEND=NACK(L=%d)", my_lc);
	}

	log_write("server.log", "client", s_command);

	return(&outdata_get);

}

// CODE THE PROPOSER WILL RUN WHEN A CLIENT SEND A GET
xdrMsg * proposer_propose(xdrMsg * indata)
{

	char s_command[BUFFSIZE];
	my_lc = my_lc + 1;

	switch (indata->command)
	{
	case RPC_PUT:
		sprintf(s_command, "RECV=PROPOSE_PUT(L=%d, K=%d, V=%d)", my_lc, indata->key, indata->value);
		break;
	case RPC_DEL:
		sprintf(s_command, "RECV=PROPOSE_DEL(L=%d, K=%d)", my_lc, indata->key);
		break;
	default:  // BAD COMMAND RETURN A NACK
		sprintf(s_command, "RECV=PROPOSE_BAD(cmd=%d, L=%d)", indata->command, my_lc);
		log_write("server.log", "client", s_command);
		sprintf(s_command, "SEND=NACK(%d, L=%d)", indata->command, my_lc);
		log_write("server.log", "client", s_command);
		outdata_propose = *indata;
		outdata_propose.status = NACK;
		return(&outdata_propose);
	}

	log_write("server.log", "client", s_command);


	xdrMsg message;
	xdrMsg response = { 0 };

	message.key     = indata->key;
	message.value   = indata->value;
	message.lc      = my_lc;
	message.pid     = 0;
	message.status  = OK;
	message.command = indata->command;

	int promise_count = 0;

	int current_status;
	xdrMsg current_result;

	// SEND PREPARE(MESSAGE) TO ACCEPTORS
	for (int i = 0; i < server_count; i++)
	{
		if (message.command == RPC_PUT)
			sprintf(s_command, "SEND=PREPARE_PUT(L=%d, K=%d, V=%d)", message.lc, message.key, message.value);
		else // it has to be delete
			sprintf(s_command, "SEND=PREPARE_DEL(L=%d, K=%d", message.lc, message.key);

		if (strcmp(myname, servers[i]) == 0)
		{   // AUTOMATICALLY ASSUME THAT ONES SELF WOULD ACTUALLY REPSPOND WITH PROMISE
			log_write("server.log", "localhost", s_command);
			current_status         = 0;
			current_result.lc      = message.lc;
			current_result.status  = PROMISE;
			current_result.key     = message.key;
			current_result.value   = message.value;
			current_result.command = message.command;
			current_result.pid     = 0;
			sprintf(s_command, "RECV=PROMISE(L=%d)", message.lc);
			log_write("server.log", "localhost", s_command);
		} else {
			log_write("server.log", servers[i], s_command);
			current_status = callrpc(servers[i],
					RPC_PROG_NUM,
					RPC_PROC_VER,
					RPC_PREPARE,
					xdr_rpc,
					&message,
					xdr_rpc,
					&response);

			current_result = response;

			if (current_result.status == PROMISE)
				sprintf(s_command, "REVC=PROMISE(L=%d)", response.lc);
			else
				sprintf(s_command, "RECV=NACK(L-%d)", response.lc);

			log_write("server.log", servers[i], s_command);

		}

		// INCREASE MY_LC IF NEED ME
		if (response.lc > my_lc)
			my_lc = response.lc;


		// NOW FIGURE OUT IF I HAVE A QUAROM
		if (current_status == 0 && current_result.status == PROMISE)  // IF WE HAVE A GOOD RESULT
		{
			promise_count = promise_count + 1;
			if (promise_count >= quarom_count)
				break;  // WE HAVE A QUAROM!
		}
	}  // GET PROMISE FROM NEXT SERVER


	if (promise_count < quarom_count)
	{
		// NO QUAROM, RESPOND TO CLIENT WITH FAILURE
		if (message.command == RPC_PUT)
			sprintf(s_command, "SEND=PUT_FAILURE(%d,%d, L=%d)", message.key, message.value, my_lc);
		else
			sprintf(s_command, "SEND=DEL_FAILURE(%d,%d, L=%d)", message.key, my_lc);

		log_write("server.log", "client", s_command);
		outdata_propose.lc = my_lc;
		outdata_propose.pid = 0;
		outdata_propose.status = NACK;
		outdata_propose.key = message.key;
		outdata_propose.value = message.value;
		outdata_propose.command = message.command;

		return(&outdata_propose);

	}

// NOW WE HAVE TO GET A QUAROM OF ACCEPTS
	promise_count = 0;
	current_status = -1;
	current_result = (xdrMsg) { 0 };
	message.status = OK;

	if (message.command == RPC_PUT)
		sprintf(s_command, "SEND=ACCEPT_PUT(L=%d, K=%d, V=%d)", message.lc, message.key, message.value);
	else
		sprintf(s_command, "SEND=ACCEPT_DEL(L=%d, K=%d)", message.lc, message.key);
	// LOOP THROUGH ALL SERVERS AND GET ACCEPTS
	for (int i = 0; i < server_count; i++)
	{
		if (strcmp(myname, servers[i]) == 0)
		{
			// ALWAYS ASSUME I WILL ACCEPT MY OWN VALUE.
			current_status = 0;
			log_write("server.log", "localhost", s_command);
			current_status         = 0;
			current_result.lc      = message.lc;
			current_result.status  = ACCEPT;
			current_result.key     = message.key;
			current_result.value   = message.value;
			current_result.command = message.command;
			current_result.pid     = 0;
			if (message.command == RPC_PUT)
				sprintf(s_command, "RECV=ACCEPTED_PUT(L=%d, K=%d, V=%d)", message.lc, message.key, message.value);
			else
				sprintf(s_command, "RECV=ACCEPTED_DEL(L=%d, K=%d)", message.lc, message.key);

			log_write("server.log", "localhost", s_command);
		} else {

			current_status = callrpc(servers[i],
					RPC_PROG_NUM,
					RPC_PROC_VER,
					RPC_ACCEPT,
					xdr_rpc,
					&message,
					xdr_rpc,
					&response);
			current_result = response;

			if (response.status == ACCEPT && message.command == RPC_PUT)
				sprintf(s_command, "RECV=ACCEPTED_PUT(L=%d, K=%d, V=%d)", message.lc, message.key, message.value);
			else if (response.status == ACCEPT && message.command == RPC_DEL)
				sprintf(s_command, "RECV=ACCEPTED_DEL(L=%d, K=%d)", message.lc, message.key);
			else
				sprintf(s_command, "RECV=NACK(L=%d)", message.lc);

			log_write("server.log", servers[i], s_command);

		}

		// IF ALL THINGS ARE EQUAL
		if (current_status == 0 && current_result.status == ACCEPT && xdr_compare(&current_result, &message) == 1)
		{
			promise_count = promise_count + 1;
			if (promise_count >= quarom_count)
				break;
		}
	}  // ASK THE NEXT SERVER


	if (promise_count < quarom_count)
	{
		// NO QUAROM, RETURN THE FAILURE MESSAGE.
		if (message.command == RPC_PUT)
			sprintf(s_command, "SEND=PUT_FAILURE(K=%d, V=%d, L=%d)", message.key, message.value, my_lc);
		else
			sprintf(s_command, "SEND=DEL_FAILURE(K=%d, L=%d)", message.key, my_lc);

		log_write("server.log", "client", s_command);
		outdata_propose.status = NACK;
		outdata_propose.key = message.key;
		outdata_propose.value = message.value;
		outdata_propose.command = message.command;
		outdata_propose.pid = 0;
		outdata_propose.lc = my_lc;
		return (&outdata_propose);
	}


	// WE HAVE A QUAROM AT THIS POINT, WITH A MAJORITY OF ACCEPTORS, SO WE JUST NEED TO TELL THEM ALL TO LEARN IT!
	for (int i = 0; i < server_count; i++)
	{
		if (strcmp(myname, servers[i]) == 0)
		{
			// IF IT IS THE SAME, JUST DO THE LEARNING YOURSELF
			if (message.command == RPC_PUT)
				sprintf(s_command, "SEND=LEARN_PUT(%d,%d, L=%d)", message.key, message.value, my_lc);
			else
				sprintf(s_command, "SEND=LEARN_DEL(%d, L=%d)", message.key, my_lc);

			log_write("server.log", "localhost", s_command);

			int result;
			if (message.command == RPC_PUT)
				result = kv_put(kv_store, indata->key, indata->value);
			else
				result = kv_del(kv_store, indata->key);

			if (result == 0)
			{
				if  (message.command == RPC_PUT)
					sprintf(s_command, "RECV=LEARN_SUCCESS(%d, %d, L=%d)", indata->key, indata->value, my_lc);
				else
					sprintf(s_command, "RECV=LEARN_SUCCESS(%d, L=%d)", indata->key, my_lc);
			} else {
				if (message.command == RPC_PUT)
					sprintf(s_command, "RECV=LEARN_FAILURE(%d, %d, L=%d)", indata->key, indata->value, my_lc);
				else
					sprintf(s_command, "RECV=LEARN_FAILURE(%d, L=%d)", indata->key, my_lc);
			}

			log_write("server.log", "localhost", s_command);

		} else {
			if (message.command == RPC_PUT)
				sprintf(s_command, "SEND=LEARN_PUT(%d,%d, L=%d)", message.key, message.value, my_lc);
			else
				sprintf(s_command, "SEND=LEARN_DEL(%d,%d, L=%d)", message.key, my_lc);

			log_write("server.log", servers[i], s_command);
			message.command = indata->command;
			message.status = OK;
			message.pid = 0;
			int status = callrpc(servers[i],
					RPC_PROG_NUM,
					RPC_PROC_VER,
					RPC_LEARN,
					xdr_rpc,
					&message,
					xdr_rpc,
					&response);
			if (status == 0 & response.status == OK)
			{
				if (message.command == RPC_PUT)
					sprintf(s_command, "RECV=LEARN_PUT_SUCCESS(%d,%d, L=%d)", response.key, response.value, my_lc);
				else
					sprintf(s_command, "RECV=LEARN_DEL_SUCCESS(%d, L=%d)", response.key, my_lc);
			} else {
				if (message.command == RPC_PUT)
					sprintf(s_command, "RECV=LEARN_PUT_FAILURE(%d,%d, L=%d)", response.key, response.value, my_lc);
				else
					sprintf(s_command, "RECV=LEARN_DEL_FAILURE(%d, L=%d)", response.key, my_lc);
			}


			log_write("server.log", servers[i], s_command);

		}
	}


	// NOW THAT ALL OF THE STUFF HAS BEEN DONE.  RETURN TO THE CLIENT.
	outdata_propose.command = message.command;
	outdata_propose.lc = my_lc;
	outdata_propose.key = message.key;
	outdata_propose.status = response.status;
	outdata_propose.value = message.value;
	outdata_propose.pid = 0;
	return(&outdata_propose);

}


//// ACCEPTER CODE
//xdrMsg * server_rpc_2pc(xdrMsg * indata) {
//
//	xdrMsg outdata = { 0 };
//	char s_command[BUFFSIZE];
//	char r_command[BUFFSIZE];
//	switch (indata->status) {
//	case PREPARE:
//		sprintf(s_command, "RECV=PREPARE(%d)", indata->key);
//		log_write("server.log", "Coordinator", s_command);
//		if (kv_get_lock_status(kv_store, indata->key) == KV_LOCKED)
//		{
//			sprintf(r_command, "SENT=NACK(%d)", indata->key);
//			outdata.status = NACK;
//		} else {
//			sprintf(r_command, "SENT=READY(%d)", indata->key);
//			kv_set_lock_status(kv_store, indata->key, KV_LOCKED);
//			outdata.status = READY;
//		}
//		break;
//
//	case COMMIT_PUT:
//		sprintf(s_command, "RECV=COMMIT_PUT(%d,%d)", indata->key, indata->value);
//		log_write("server.log", "Coordinator", s_command);
//		kv_put(kv_store, indata->key, indata->value);
//		kv_set_lock_status(kv_store, indata->key, KV_UNLOCKED);
//		outdata.status = OK;
//		sprintf(r_command, "SENT=OK(%d)", indata->key);
//		break;
//
//	case COMMIT_DEL:
//		sprintf(s_command, "RECV=COMMIT_DEL(%d)", indata->key);
//		log_write("server.log", "Coordinator", s_command);
//		kv_set_lock_status(kv_store, indata->key, KV_UNLOCKED);
//		kv_del(kv_store, indata->key);
//		outdata.status = OK;
//		sprintf(r_command, "SENT=OK(%d)", indata->key);
//		break;
//
//	case ABORT:
//		sprintf(s_command, "RECV=ABORT(%d)", indata->key);
//		log_write("server.log", "Coordinator", s_command);
//		kv_set_lock_status(kv_store, indata->key, KV_UNLOCKED);
//		outdata.status = OK;
//		sprintf(r_command, "SENT=OK(%d)", indata->key);
//		break;
//
//	default:
//		sprintf(s_command, "RECV=BADCOMMAND");
//		log_write("server.log","Coordinator", s_command);
//		outdata.status = NACK;
//		sprintf(r_command, "SENT=NACK");
//		break;
//	}
//
//	// RESPOND WITH MESSAGE.
//	log_write("server.log","Coordinator", r_command);
//	return (&outdata);
//
//}

/*********************************************************
 * RPC FUNCTION FOR RESPONDING TO RPC CALLS FOR DELETING *
 * A VALUE FROM THE KEY VALUE STORE.  INDATA IS THE      *
 * MESSAGE PROVIDED BY THE RPC CALLER.                   *
 *******************************************************/
/********************************************************
 * THE FUNCTION CALLED BY THE MAIN MENU THAT SETSUP THE *
 * RPC FUNCTIONS AND BEGINS TO LISTEN FOR INCOMING      *
 * MEESAGES. DOES NOT RETURN.                           *
 *******************************************************/
///******************************************
// * INTERPRETS THE MESSAGE RECEIVED BY THE *
// * SERVER FROM THE CLIENT AND DETERMINES  *
// * WHAT ACTION TO TAKE.  IF THE MESSAGE IS*
// * MALFORMED IT WILL RETURN -1 OTHERWISE  *
// * THE RESPONSE WILL BE STORED IN THE     *
// * BUFFER PROVIDED.                       *
// *****************************************/
//int server_handle_message(char* msg, char* response) {
//	int command = 0;
//	int key = 0;
//	int value = 0;
//	int r = kv_parser(msg, &command, &key, &value);
//
//	if (r < 0) {
//		sprintf(response, "Invalid Message %d: %s", r, msg);
//		return (-1);  //MALFORMED
//	}
//
//	int s = 0;
//	switch (command) {
//	case 0:
//		s = kv_put(kv_store, key, value);
//		if (s == 0)
//			sprintf(response, "Key Stored Successfully");
//		else
//			sprintf(response, "Error storing value %d into key %d.", value,
//					key);
//		break;
//	case 1: //GET
//		s = kv_get(kv_store, key, &value);
//		if (s == 0)
//			sprintf(response, "%d", value);
//		else
//			sprintf(response, "Value not found");
//		break;
//	case 2: //DEL
//		s = kv_del(kv_store, key);
//		if (s == 0)
//			sprintf(response, "Deleted Successfully.");
//		else
//			sprintf(response, "Error Removing Value");
//		break;
//
//	default:
//		sprintf(response, "Unable to interpret command");
//		return (-1); //BAD COMMAND
//		break;
//	}
//
//	return (0);
//}


void chaos_function()
{
	int r = (rand() % 100);
	if (r < FAIL_RATE)
	{
		log_write("server.log", myname, "!!!!!!!SYSTEM_FAILURE!!!!!");
		exit(-1);
//		printf("\n");
//		nanosleep(&fail, NULL);
//		printf("\n");
//		log_write("server.log", myname, "!!!!!!!SYSTEM_RESUMED!!!!!");
	}
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
