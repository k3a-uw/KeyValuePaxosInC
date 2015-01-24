/*
 ============================================================================
 Name        : client.h
 Author      : Kevin Anderson <k3a@uw.edu> & Daniel Kristiyanto <danielkr@uw.edu>
 Version     : 2015.01.18
 ============================================================================
 */


#ifndef CLIENT_H
#define CLIENT_H


#ifndef _STDIO_H_
  #include <stdio.h>
#endif

#ifndef _SYS_TYPES_H_
  #include <sys/types.h>
#endif

#ifndef _SYS_SOCKET_H_
  #include <sys/socket.h>
#endif

#ifndef _NETINET_IN_H_
  #include <netinet/in.h>
#endif

#ifndef INET_H
#include <arpa/inet.h>
#endif

#ifndef _STLIB_H_
  #include <stdlib.h>
#endif

#ifndef _STRING_H_
  #include <string.h>
#endif

#ifndef _STDLIB_H_
  #include <stdlib.h>
#endif

#ifndef BUFFSIZE
  #define BUFFSIZE 128
#endif

#ifndef LOG_H
  #include "log.h"
#endif

/********************************************************
 *  LAUNCHES A SCRIPT THAT COMMUNICATES WITH THE SERVER *
 *  INDICATED, OVER THE PORT_NUMBER PROVIDED.  CALLS    *
 *  getMessages() TO DETERMINE WHAT COMMANDS TO SEND TO *
 *  THE SERVER. EXITS PROGRAM WHEN COMPLETED.           *
 *  THE PROTOCOL USED: UDP                              *
 *******************************************************/
int client_udp_init(char* hostname, unsigned short port_num);

/********************************************************
 *  LAUNCHES A SCRIPT THAT COMMUNICATES WITH THE SERVER *
 *  INDICATED, OVER THE PORT_NUMBER PROVIDED.  CALLS    *
 *  getMessages() TO DETERMINE WHAT COMMANDS TO SEND TO *
 *  THE SERVER. EXITS PROGRAM WHEN COMPLETED.           *
 *  THE PROTOCOL USED: TCP                              *
 *******************************************************/
int client_tcp_init(char* hostname, unsigned short port_num);

/*****************************************************
 * RETURNS A CHAR* ARRAY THAT CONTAINTS A SERIES OF  *
 * MESSAGES THAT ARE TO BE SENT TO THE SERVER        *
 * **************************************************/
void getMessages(char* messages[]);

/*********************************************
 * WRITES ERROR TO THE CONSOLE AND EXITS THE *
 * SYSTEM WITH AN ERROR CODE OF -1           *
 ********************************************/
void ClientErrorHandle(char* errormessage);

#endif
