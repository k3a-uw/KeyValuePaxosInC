/*
 ============================================================================
 Name        : server.h
 Author      : Kevin Anderson <k3a@uw.edu> & Daniel Kristiyanto <danielkr@uw.edu>
 Version     : 2015.01.18
 Description : Listens and handles incoming messages from a client.  It will
             : add, put and delete values into a key value store.  Runs
             : forever until a kill or ctrl+c command is ran.
 ============================================================================
 */

#ifndef SERVER_H
#define SERVER_H
#define MAXPENDING 5    /* Maximum outstanding connection requests */
#define BUFFSIZE 128    /* The size of the incoming and outgoing messages.*/
#define THREAD_COUNT 10

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <string.h>     /* for memset() */
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <rpc/rpc.h>
#include <utmp.h>


#ifndef KEYVALUE_H
  #include "keyvalue.h"
#endif

#ifndef LOG_H
#include "log.h"
#endif


#ifndef MESSAGEQUEUE_H
#include "messagequeue.h"
#endif

/******************************************
 * LAUNCHES A TCP SERVER TO LISTEN ON THE *
 * PORT PROVIDED.  RUNS UNTIL PROCES IS   *
 * KILLED.  WILL FAIL IF SOCKETS ARE      *
 * UNABLE TO BE CREATED, BOUND, ETC.      *
 *****************************************/
int server_tcp_init(unsigned short port_num);

/******************************************
 * LAUNCHES A UDP SERVER TO LISTEN ON THE *
 * PORT PROVIDED.  RUNS UNTIL PROCES IS   *
 * KILLED.  WILL FAIL IF SOCKETS ARE      *
 * UNABLE TO BE CREATED, BOUND, ETC.      *
 *****************************************/
int server_udp_init(unsigned short port_num);

/******************************************
 * INTERPRETS THE MESSAGE RECEIVED BY THE *
 * SERVER FROM THE CLIENT AND DETERMINES  *
 * WHAT ACTION TO TAKE.  IF THE MESSAGE IS*
 * MALFORMED IT WILL RETURN -1 OTHERWISE  *
 * THE RESPONSE WILL BE STORED IN THE     *
 * BUFFER PROVIDED.                       *
 *****************************************/
int server_handle_message(char* msg, char* response);


/**********************************
 * WRITES AN ERROR TO THE CONSOLE *
 * AND EXIST THE PROGRAM          *
 *********************************/
void ServerErrorHandle(char *errorMessage);


/***********************************
 * AN AGENT FOR A THREAD THAT WILL *
 * CHECK THE QUEUE FOR ACTION AND  *
 * PERFORM IT IF NECESSARY         *
 ***********************************
 */
void *MessageAgent(void* mq);

/******************************************
 * A HELPER METHOD TO INTERPRET THE COMMAND
 * AND RESPOND AS NECCESSARY
 *****************************************/
void server_read_and_respond(char* message, char* client);

void server_respond(char* message, char* client);



#endif /* SRC_SERVER_H_ */
