/*
 * xdrconv.h
 *
 *  Created on: Jan 28, 2015
 *      Author: daniel
 */


// XDR CONVERSION

#ifndef XDRCONV_H
#define XDRCONV_H

#define RPC_PUT        1
#define RPC_GET        2
#define RPC_DEL        3
#define RPC_PROG_NUM   0x20000001
#define RPC_PROC_VER   1

#include <rpc/rpc.h>



/*******************************************************
 * DEFINES THE DATA STRUCTURE FOR MESSAGES				*
 * TRANSMITTED FROM AND TO CLIENT VIA RPC				*
 * 														*
 ******************************************************/

typedef struct msgRpc{
	int key;
	int value;
} xdrMsg;

int xdr_rpc(XDR* xdr, xdrMsg* content);

#endif /* SRC_XDRCONV_H_ */
