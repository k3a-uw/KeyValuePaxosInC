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
#include "xdrconv.h"
#endif


/*******************************************************
 * USER-DEFINED EXTERNAL DATA REPRESENTATION 			*
 * FOR RPC COMMUNICATION BETWEEN CLIENT AND SERVER		*
 * FOR THE xdrMSG DATA TYPE 							*
 ******************************************************/
int xdr_rpc(XDR * xdr, xdrMsg * content)
{
		if (!xdr_int(xdr, &content->key))
		              return (0);
		if (!xdr_int(xdr, &content->value))
		              return (0);
		if (!xdr_int(xdr, &content->status))
					  return (0);

		return (1);
}


int xdr_compare(xdrMsg * a, xdrMsg * b)
{
	// if key, value and command are the same, return 1 else -1
}
