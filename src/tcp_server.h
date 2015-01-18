/*
 * server.h
 *
 *  Created on: Jan 10, 2015
 *      Author: kevanderson
 */

#ifndef TCP_SERVER_H
  #define TCP_SERVER_H


#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <string.h>     /* for memset() */
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include "keyvalue.h"
#include <netdb.h>
#include <unistd.h>
#include "log.h"

#ifndef MAXPENDING
  #define MAXPENDING 5    /* Maximum outstanding connection requests */
  #define BUFFSIZE 128
#endif

int tcp_server_main(int argc, char *argv[]);


#endif
