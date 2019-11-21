/*******************************************************************************
 *
 * __doc__ \subsection {dhsUtilSysClose}
 * __doc__ \begin{description}
 * __doc__  \item[\sc use:] \emph{void dhsUtilSysClose(long $\star$istat, char $\star$resp, dhsHandle dhsID);}
 * __doc__  \item[\sc description:] used by the NOCS and MSL to open a connection to the DHS.
 * __doc__  \item[\sc argument(s):]
 * __doc__   \begin{description}
 * __doc__    \item[\ ] \ 
 * __doc__    \item[\emph{long $\star$istat}] the inherited status value.
 * __doc__    \item[\emph{char $\star$resp}] a user supplied buffer for message strings.
 * __doc__   \item[\emph{dhsHandle dhsID}] dhs handle.
 * __doc__   \end{description}
 * __doc__  \item[\sc return(s):] void.
 * __doc__  \item[\sc author(s):] Peter Ruckle, Phil Daly
 * __doc__  \item[\sc last modified:] 
 * __doc__  \item[\sc license:] (c) 2004 AURA Inc. All rights reserved. Released under the GPL.
 * __doc__ \end{description}
 *
 ******************************************************************************/

/*******************************************************************************
 * include(s):
 *******************************************************************************/
#if !defined(_dhsUtil_H_)
 #include "dhsUtil.h"
#endif
#if !defined(_dhsImpl_H_)
 #include "dhsImplementationSpecifics.h"
#endif 

/*******************************************************************************
 * dhsUtilSysClose ( ... )
 *******************************************************************************/ 
void dhsUtilSysClose ( long *istat,     /* inherited status                    */
		       char *resp,      /* response message                    */
		       dhsHandle dhsID  /* dhs handle                          */
    ) 
{
    /* check the inherited status */
    if ( STATUS_BAD(*istat) ) return;
    MNSN_CLEAR_STR(resp,'\0');
    
    /* check input values */
    if ( dhsID < (dhsHandle)0 ) 
    {
	*istat = DHS_ERROR;
	MNSN_RET_FAILURE(resp,"dhsSysClose:  bad parameter, dhsHandle invalid");
	return;
    }

    /* for this library, this is a no-op */
    *istat = DHS_OK;
    MNSN_RET_SUCCESS(resp,"dhsSysClose:  Success.");
    /* return */
    return;
}
