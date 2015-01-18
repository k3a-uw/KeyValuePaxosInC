/*
 * client.h
 *
 *  Created on: Jan 18, 2015
 *      Author: kevanderson
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

int client_udp_init(char* hostname, unsigned short port_num);

int client_tcp_init(char* hostname, unsigned short port_num);

void getMessages(char* messages[]);

void ClientErrorHandle(char* errormessage);

#endif
