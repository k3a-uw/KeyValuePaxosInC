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

#ifndef CLIENT_H
  #include "client.h"
#endif

#ifndef SERVER_H
  #include "server.h"
#endif

#ifndef _STDIO_H_
  #include <stdio.h>
#endif

#ifndef _STDLIB_H_
  #include <stdlib.h>
#endif


int main(int argc, char *argv[]);
int validateCommandLine(int argc, char * argv[], char* hostname, unsigned short* port_num);
int validatePort(char* arg);
void printMenu(int argc);

#endif /* MAIN_H */
