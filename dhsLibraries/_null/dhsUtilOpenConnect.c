/*******************************************************************************
 *
 * __doc__ \subsection {dhsUtilOpenConnect}
 * __doc__ \begin{description}
 * __doc__  \item[\sc use:] \emph{void dhsUtilOpenConnect(long $\star$istat, char $\star$resp, dhsHandle $\star$dhsID, ulong whoami, fpConfig\_t $\star$cfg);}
 * __doc__  \item[\sc description:] open a connection to the DHS.
 * __doc__  \item[\sc argument(s):]
 * __doc__   \begin{description}
 * __doc__    \item[\ ] \ 
 * __doc__    \item[\emph{long $\star$istat}] the inherited status value.
 * __doc__    \item[\emph{char $\star$resp}] a user supplied buffer for message strings.
 * __doc__   \item[\emph{dhsHandle $\star$dhsID}] returned dhs handle.
 * __doc__   \item[\emph{ulong whoami}] identifier for NOCS or MSL or other system making the call.
 * __doc__   \item[\emph{fpConfig\_t $\star$cfg}] focal plane configuration.
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
 * dhsUtilOpenConnect ( ... )
 *******************************************************************************/ 
void dhsUtilOpenConnect (
  long *istat,      /* inherited status                                        */
  char *resp,       /* response message                                        */
  dhsHandle *dhsID, /* returned handle                                         */
  ulong whoami,     /* identifier for mid-level system                         */
  fpConfig_t *cfg   /* focal plane configuration                               */
) {

  /* check the inherited status */
  if ( STATUS_BAD(*istat) ) return;
  MNSN_CLEAR_STR(resp,'\0');
    
  /* check input parameters */
  if ( dhsID==(dhsHandle *)NULL ) {
    *istat = DHS_ERROR;
    MNSN_WRITE_VAL(resp,"ERROR","dhsOpenConnect-E-bad parameter",*istat);
    return;
  }

  /* check input values */
  if ( whoami==DHS_IAMOCS || whoami==DHS_IAMMSL ) {
    *istat = DHS_ERROR;
    MNSN_WRITE_VAL(resp,"ERROR","dhsOpenConnect-E-bad value",*istat);
    return;
  }

  /* for this library, this is a no-op */
  *dhsID = (dhsHandle) DHS_SIMULATION_OK;
  *istat = DHS_OK;
  MNSN_WRITE_VAL(resp,"OK","dhsOpenConnect-I-succeeded",*istat);

  /* return */ 
  return;
}
