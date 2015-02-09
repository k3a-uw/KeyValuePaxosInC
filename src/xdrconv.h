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

#define RPC_PUT        1
#define RPC_GET        2
#define RPC_DEL        3
#define RPC_2PC        4
#define RPC_PROG_NUM   0x20000001
#define RPC_PROC_VER   1

#define NACK          -1
#define FAILURE       -2
#define ABORT         -3

#define COMMIT_PUT     1
#define COMMIT_DEL     2
#define PREPARE        3
#define READY          4
#define OK             6


/********************************************************
 * DEFINES THE DATA STRUCTURE FOR MESSAGES				*
 * TRANSMITTED FROM AND TO CLIENT VIA RPC				*
 *******************************************************/
typedef struct msgRpc{
	int key;
	int value;
	int status;
} xdrMsg;

int xdr_rpc(XDR* xdr, xdrMsg* content);

#endif
