/*
 ============================================================================
 Name        : main.h
 Author      : Kevin Anderson <k3a@uw.edu> & Daniel Kristiyanto <danielkr@uw.edu>
 Version     : 2015.01.10
 Description : TCSS558 Project 1
 ============================================================================
 */

#ifndef MAIN_H
  #define MAIN_H
  #define MEMORY_ALLOCATION_ERROR -255
#endif

#ifndef _STDIO_H_
  #include <stdio.h>
#endif

#ifndef _STDLIB_H_
  #include <stdlib.h>
#endif

#ifndef _STRING_H_
  #include <string.h>
#endif

#ifndef UDP_CLIENT_H
  #include "udp_client.h"
#endif

#ifndef UDP_SERVER_H
  #include "udp_server.h"
#endif

#ifndef TCP_CLIENT_H
  #include "tcp_client.h"
#endif

#ifndef TCP_SERVER_H
  #include "tcp_server.h"
#endif

#ifndef LOG_H
  #include "log.h"
#endif

#ifndef KEYVALUE_H
  #include "keyvalue.h"
#endif

int main(int argc, char *argv[]);
