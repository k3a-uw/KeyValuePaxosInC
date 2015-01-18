/*
 * server.h
 *
 *  Created on: Jan 18, 2015
 *      Author: kevanderson
 */

#ifndef SERVER_H
#define SERVER_H
#define MAXPENDING 5    /* Maximum outstanding connection requests */
#define BUFFSIZE 128    /* The size of the incoming and outgoing messages.*/


#ifndef _STDIO_H_
  #include <stdio.h>
#endif

#ifndef _STDLIB_H_
  #include <stdlib.h>
#endif

#ifndef _SYS_SOCKET_H_
  #include <sys/socket.h> /* for socket(), bind(), and connect() */
#endif

#ifndef _ARPA_INET_H_
  #include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#endif

#ifndef _STRING_H_
  #include <string.h>     /* for memset() */
#endif

#ifndef _SYS_TYPES_H
  #include <sys/types.h>
#endif

#ifndef _NETDB_H_
#include <netdb.h>
#endif

#ifndef _UNISTD_H_
#include <unistd.h>
#endif

#ifndef KEYVALUE_H
  #include "keyvalue.h"
#endif

#ifndef LOG_H
#include "log.h"
#endif

int server_tcp_init(unsigned short port_num);
int server_udp_init(unsigned short port_num);
int server_handle_message(kv* kv_store, char* msg, char* response);
void ServerErrorHandle(char *errorMessage);


#endif /* SRC_SERVER_H_ */
