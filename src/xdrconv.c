/*
 * xdrconv.c
 *
 *  Created on: Jan 28, 2015
 *      Author: daniel
 */
#ifndef XDRCONV_H
#include "xdrconv.h"
#endif

// XDR CONVERSION

/*******************************************************
 * USER-DEFINED EXTERNAL DATA REPRESENTATION 			*
 * FOR RPC COMMUNICATION BETWEEN CLIENT AND SERVER		*
 * FOR THE xdrMSG DATA TYPE 							*
 ******************************************************/

int xdr_rpc(xdr, content)
	XDR *xdr;
	struct msgRpc *content;
{
		if (!xdr_int(xdr, &content->key))
		              return (0);
		if (!xdr_int(xdr, &content->value))
		              return (0);
		return (1);
}
