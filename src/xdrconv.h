/*
 * xdrconv.h
 *
 *  Created on: Jan 28, 2015
 *      Author: daniel
 */


// XDR CONVERSION

#ifndef XDRCONV_H
#define XDRCONV_H
#include <rpc/rpc.h>



/*******************************************************
 * DEFINES THE DATA STRUCTURE FOR MESSAGES				*
 * TRANSMITTED FROM AND TO CLIENT VIA RPC				*
 * 														*
 ******************************************************/

typedef struct msgRpc{
	int key;
	int value;
}xdrMsg;

int xdr_rpc(xdr, content);


#endif /* SRC_XDRCONV_H_ */
