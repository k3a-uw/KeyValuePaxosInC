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
		if (!xdr_int(xdr, &content->command))
					  return (0);
		if (!xdr_int(xdr, &content->lc))
					  return(0);
		if (!xdr_int(xdr, &content->pid))
		              return (0);

		return (1);
}


int xdr_compare(xdrMsg * a, xdrMsg * b)
{
//	printf("Looking at the compare:\n a->key = %d \t b->key = %d \n a->value = %d \t b->value = %d \n a->cmd = %d \t b->cmd = %d \n",
//			a->key, b->key, a->value, b->value, a->command, b->command);

	if (a->key     == b->key
	&&  a->value   == b->value
	&&	a->command == b->command
	)
	{
		return(1);
	} else {
		return(-1);
	}
}
