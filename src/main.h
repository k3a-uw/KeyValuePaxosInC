/*
 ============================================================================
 Name        : main.h
 Author      : Kevin Anderson <k3a@uw.edu> & Daniel Kristiyanto <danielkr@uw.edu>
 Version     : 2015.01.18
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

/*******************************************************
 * DETERMINES SERVER OR CLIENT BASED ON ARGUMENTS AND  *
 * PROMPTS THE USER FOR THE DESIRED PROTOCOL.  IF THE  *
 * ARGUMENTS ARE BAD, THE SYSTEM FILL FAIL.            *
 ******************************************************/
int main(int argc, char *argv[]);

/********************************************
 * VALIDATES AND EXTRACTS THE HOSTNAME AND  *
 * PORT NUMBER FROM THE COMMAND LINES.  IF  *
 * THE PORT NAME IS BAD, IT WILL RETURN -1  *
 * HOSTNAME AND PORTS ARE STORED AT THE     *
 * ADDRESSES PROVIDED                       *
 * *****************************************/
int validateCommandLine(int argc, char * argv[], char* hostname, unsigned short* port_num);

/**************************************
 * VERIFIES THAT THE PORT PROVIDED IS *
 * AN INTEGER BETWEEN 65534 AND 100   *
 * INCLUSIVE.  RETURNS 0 IF TRUE, AND *
 * -1 IF FALSE                        *
 *************************************/
int validatePort(char* arg);

/***************************************
 * DRAWS THE APPROPRIATE MENU BASED ON *
 * THE NUMBER OF ARGUMENTS PROVIDED.   *
 * IF THE NUMBER OF ARGUMENTS IS 2, IT *
 * IS ASSUMED TO BE THE SERVER, AND IF *
 * THERE ARE MORE THAN 2, IT IS ASSUMED*
 * TO BE A CLIENT CONNECTION.  THE     *
 * ADDITIONAL ARGUMENTS ARE IGNORED    *
 * ************************************/
void printMenu(int argc);

#endif /* MAIN_H */
