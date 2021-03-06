/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "chronos.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

chronos_out *
chronosproc_1(chronos_in *argp, CLIENT *clnt)
{
	static chronos_out clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, chronosPROC,
		(xdrproc_t) xdr_chronos_in, (caddr_t) argp,
		(xdrproc_t) xdr_chronos_out, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}
