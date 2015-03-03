/*
 ============================================================================
 Name        : xdrconv.c
 Author      : Daniel Kristiyanto <danielkr@uw.edu>
 Version     : 2015.01.28
 Description : Converts the messages passed between client and server into
             : An XDR transmission.
 ============================================================================
 */
#ifndef XDRCONV_H
#define XDRCONV_H

#include <rpc/rpc.h>

// GENERAL, ALL PURPOSE
#define RPC_PROG_NUM   0x20000001
#define RPC_PROC_VER   1


// TO DELETE
#define RPC_2PC        4  // DON'T NEED
#define COMMIT_PUT     1  // CAN DELETE LATER
#define COMMIT_DEL     2  // CAN DELETE LATER
#define READY          5  // CAN DELETE LATER
#define OK             6

// CLIENT TO PROPOSER
#define RPC_GET        1
#define RPC_PUT        2
#define RPC_DEL        3

// PROPOSER TO ACCEPTOR
#define RPC_PREPARE    5
#define RPC_ACCEPT     6

// PROPOSER TO LEARNER
#define RPC_LEARN_GET  7

// ACCEPTOR TO LEARNER
#define RPC_LEARN      8

// GENERAL MESSAGE TYPES
#define NACK          -1
#define FAILURE       -2
#define ABORT         -3
#define PREPARE        3
#define PROMISE        4
#define ACK            6
#define ACCEPT         7
#define LEARN          8



/********************************************************
 * DEFINES THE DATA STRUCTURE FOR MESSAGES				*
 * TRANSMITTED FROM AND TO CLIENT VIA RPC				*
 *******************************************************/
typedef struct msgRpc{
	int key;      // the 'key' of the message (used in put,get,del)
	int value;    // the 'value' of the message (used in put only)
	int status;   // the 'type' of message NACK, PREPARE, ETC
	int command;  // the command to execute
	int lc;   // lamport clock of message
	int pid;  // process id
} xdrMsg;

int xdr_rpc(XDR* xdr, xdrMsg* content);
int xdr_compare(xdrMsg * a, xdrMsg * b);

#endif
