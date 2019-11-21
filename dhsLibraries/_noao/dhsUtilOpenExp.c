/*******************************************************************************
 *
 * __doc__ \subsection {dhsUtilOpenExp}
 * __doc__ \begin{description}
 * __doc__  \item[\sc use:] \emph{void dhsUtilOpenExp(long $\star$istat, char $\star$resp, dhsHandle dhsID, fpConfig\_t $\star$cfg, double $\star$expID, char $\star$obsetID);}
 * __doc__  \item[\sc description:] open an exposure connection to the DHS.
 * __doc__  \item[\sc argument(s):]
 * __doc__   \begin{description}
 * __doc__    \item[\ ] \ 
 * __doc__    \item[\emph{long $\star$istat}] the inherited status value.
 * __doc__    \item[\emph{char $\star$resp}] a user supplied buffer for message strings.
 * __doc__   \item[\emph{dhsHandle dhsID}] dhs handle.
 * __doc__   \item[\emph{fpConfig\_t $\star$cfg}] focal plane configuration.
 * __doc__   \item[\emph{double $\star$expID}] exposure identifier.
 * __doc__   \item[\emph{char $\star$obsetID}] observation set identifier.
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
 * dhsUtilOpenExp ( ... )
 *******************************************************************************/ 
void dhsUtilOpenExp (
  long *istat,     /* inherited status                                         */
  char *resp,      /* response message                                         */
  dhsHandle dhsID, /* dhs handle                                               */
  fpConfig_t *cfg, /* focal plane configuration                                */
  double *expID,   /* exposure identifier                                      */
  char *obsetID    /* observation set identifier                               */
) {

  /* check the inherited status */
  if ( STATUS_BAD(*istat) ) return;
  MNSN_CLEAR_STR(resp,'\0');
    
  /* check input parameters */
  if ( cfg==(fpConfig_t *)NULL || expID==(double *)NULL || obsetID==(char *)NULL ) {
    *istat = DHS_ERROR;
    MNSN_WRITE_VAL(resp,"ERROR","dhsOpenExp-E-bad parameter",*istat);
    return;
  }

  /* check input values */
  if ( dhsID < (dhsHandle)0 ) {
    *istat = DHS_ERROR;
    MNSN_WRITE_VAL(resp,"ERROR","dhsOpenExp-E-bad value",*istat);
    return;
  }

  /* for this library, this is a no-op */
  *istat = DHS_OK;
  MNSN_WRITE_VAL(resp,"OK","dhsOpenExp-I-succeeded",*istat);

  /* return */
  return;
}
