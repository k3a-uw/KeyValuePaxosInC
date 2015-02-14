/*
 ============================================================================
 Name        : client.h
 Author      : Kevin Anderson <k3a@uw.edu> & Daniel Kristiyanto <danielkr@uw.edu>
 Version     : 2015.01.18
 ============================================================================
 */


#ifndef CLIENT_H
#define CLIENT_H


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <rpc/rpc.h>
#include <utmp.h>
#include <ctype.h>
#ifndef BUFFSIZE
  #define BUFFSIZE 128
#endif

#ifndef XDRCONV_H
  #include "xdrconv.h"
#endif


#ifndef LOG_H
  #include "log.h"
#endif


/*******************************************************
 * SENDS A MESSAGE/COMMAND TO THE SERVER PROVIDED AS   *
 * HOSTNAME.  THE RESPONSE FROM THE SERVER IS STORED   *
 * IN THE ADDRESS PROVIDED IN THE VARIABLE 'RESPONSE'  *
 * IF THE COMMAND IS POORLY FORMED, NO MESSAGE WILL    *
 * BE TRANSMITTED.  THIS FUNCTION ALSO MANAGES CALLING *
 * THE LOG_WRITE FUNCTION TO ENSURE TRANSACTIONS ARE   *
 * WRITTEN TO LOG FILES.                               *
 ******************************************************/
int client_rpc_send(char* hostname, int command, xdrMsg * message, xdrMsg * response);


/***********************************************
 * CALLED BY THE MAIN FUNCTION AND INITIALIZES *
 * COMMUNICATION WITH THE SERVER PROVIDED AS   *
 * HOSTNAME.  THIS MEANS SIMPLY SETTING UP A   *
 * LIST OF COMMANDS AND MAKING CALLS TO        *
 * CLIENT_RPC_SEND                             *
 **********************************************/
int client_rpc_init(char** server_list, int server_count);


/********************************************************
 *  LAUNCHES A SCRIPT THAT COMMUNICATES WITH THE SERVER *
 *  INDICATED, OVER THE PORT_NUMBER PROVIDED.  CALLS    *
 *  getMessages() TO DETERMINE WHAT COMMANDS TO SEND TO *
 *  THE SERVER. EXITS PROGRAM WHEN COMPLETED.           *
 *  THE PROTOCOL USED: UDP
 ********************************************************/
int client_udp_init(char* hostname, unsigned short port_num);


/********************************************************
 *  LAUNCHES A SCRIPT THAT COMMUNICATES WITH THE SERVER *
 *  INDICATED, OVER THE PORT_NUMBER PROVIDED.  CALLS    *
 *  getMessages() TO DETERMINE WHAT COMMANDS TO SEND TO *
 *  THE SERVER. EXITS PROGRAM WHEN COMPLETED.           *
 *  THE PROTOCOL USED: TCP
 ********************************************************/
int client_tcp_init(char* hostname, unsigned short port_num);


/*****************************************************
 * SAVES A CHAR* ARRAY THAT CONTAINTS A SERIES OF    *
 * MESSAGES THAT ARE TO BE SENT TO THE SERVER        *
 * **************************************************/
void getMessages(char* messages[]);


/************************************************
 * SAVES AN ARRAY OF MESSAGES TO BE SENT TO THE *
 * SERVER AS A DEMO OF THE SYSTEM WORKING       *
 * AND IS TO BE USED IN CONJUNCTION WITH THE    *
 * FUNCTION GETRPCCOMMANDS.                     *
 ***********************************************/
void getRPCMessages(xdrMsg* messages);


/************************************************
 * SAVES AN ARRAY OF COMMANDS TO BE SENT TO THE *
 * SERVER AS A DEMO OF THE SYSTEM WORKING       *
 * AND IS TO BE USED IN CONJUNCTION WITH THE    *
 * FUNCTION GETRPCMESSAGES.                     *
 ***********************************************/
void getRPCCommands(int* commands);


/*********************************************
 * WRITES ERROR TO THE CONSOLE AND EXITS THE *
 * SYSTEM WITH AN ERROR CODE OF -1           *
 ********************************************/
void ClientErrorHandle(char *errorMessage);


/******
 * RUNS THE CLIENT FOR THE INTERFACE.
 */
int client_ui(char** servers, int server_count);

/*****************************************************
 * ACCEPTS AN INTEGER AND RESPONDS WITH A CHAR ARRAY *
 * WITH A HUMAN READABLE VERSION OF THAT COMMAND     *
 * **************************************************/
char* client_ui_get_command_word(int command);

/*****************************************************
 * PART OF THE UI.  ASKS THE USER FOR A COMMAND, GET
 * PUT DELETE OR SCRIPT.  IT APPLIES THE COMMAND TO THE
 * PROVIDED INT AND RETURNS -1 IF THE USER RESPONDS
 * INCORRECTLY. 0 OTHERWISE.
 * **************************************************/
int client_ui_get_command(int* command);


/*****************************************************
 * PART OF THE UI.  ASKS THE USER FOR A KEY AND
 * TELLS THE USER NOT TO USE -1 IF THEY DO.  IT SAVES
 * THE KEY TO THE PROVIDED INT AND RETURNS -1 IF THE
 * USER RESPONDS INCORRECTLY, 0 OTHERWISE
 ***************************************************/
int client_ui_get_key(int* key);


/*****************************************************
 * PART OF THE UI.  ASKS THE USER FOR A KEY AND SAVES
 * THE VALUE TO THE INT PROVIDED.  IF THE USER REPSONDS
 * INCORRECTLY, IT WILL RETURN -1. 0 OTHERWISE
 * ***************************************************/
int client_ui_get_value(int* value);


/*****************************************************
 * HOLDS WAITING FOR INPUT FROM THE USER AND PARSES
 * AN INTEGER FROM THEM.  IF WILL SAVE THE INTEGER
 * IN THE ADDRESS PROVIDED, AND RETURN -1 IF THE USER
 * DOES NOT ENTER A VALID INTEGER. 0 OTHERWISE
 * **************************************************/
int client_ui_get_int_from_user(int * input);


/***********************************************************
 * DISPLAYS A LIST OF SERVERS FROM THE CHAR ARRAY PROVIDED.
 * ASKS THE USER TO SELECT A SERVER FROM THAT LIST AND SAVES
 * THE ARRAY VALUE OF THAT SERVER (INPUT - 1).  WILL STORE
 * THE INDEX INTO THE INTEGER PROVIDED, AND RETURN -1 IF
 * THE USER ADDRESSES THE QUESTION INCORRECTLY.  REQUIRES
 * THE PROGRAMMER TO PROVIDE THE SERVER LIST AND THE COUNT OF
 * SERVERS WITHIN THAT LIST.
 ************************************************************/
int client_get_server(int * index, char** server_list, int server_count);


/***********************************************************
 * ACCEPTS A CHAR BUFFER FROM THE USER OF THE PROVIDED LENGTH
 * AND RETURNS THE INTEGER VALUE OF THAT CHAR.  IF THE STRING
 * DOES NOT CONTAIN AN INTEGER, -1 IS RETURNED AND -1 IS
 * SAVED TO THE ADDRESS PROVIDED AS AN ERROR.
 ************************************************************/
int client_parse_int(char * input, int length, int * err);


#endif
