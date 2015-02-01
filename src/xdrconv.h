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
#define RPC_PROG_NUM   0x20000001
#define RPC_PROC_VER   1




/********************************************************
 * DEFINES THE DATA STRUCTURE FOR MESSAGES				*
 * TRANSMITTED FROM AND TO CLIENT VIA RPC				*
 *******************************************************/
typedef struct msgRpc{
	int key;
	int value;
} xdrMsg;

int xdr_rpc(XDR* xdr, xdrMsg* content);

#endif
