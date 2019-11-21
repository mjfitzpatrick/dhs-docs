/*******************************************************************************
 *
 * __doc__ \subsection {dhsUtilIOCtl}
 * __doc__ \begin{description}
 * __doc__  \item[\sc use:] \emph{void dhsUtilIOCtl(long $\star$istat, char $\star$resp, dhsHandle dhsID, ulong ioctlFunction, double $\star$expID, char $\star$obsetID, ...);}
 * __doc__  \item[\sc description:] send ioctl data to the DHS.
 * __doc__  \item[\sc argument(s):]
 * __doc__   \begin{description}
 * __doc__    \item[\ ] \ 
 * __doc__    \item[\emph{long $\star$istat}] the inherited status value.
 * __doc__    \item[\emph{char $\star$resp}] a user supplied buffer for message strings.
 * __doc__   \item[\emph{dhsHandle dhsID}] dhs handle.
 * __doc__   \item[\emph{ulong ioctlFunction}] ioctl function.
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
 * dhsUtilIOCtl ( ... )
 *******************************************************************************/ 
void dhsUtilIOCtl (
  long *istat,         /* inherited status                                     */
  char *resp,          /* response message                                     */
  dhsHandle dhsID,     /* dhs handle                                           */
  ulong ioctlFunction, /* ioctl function                                       */
  double *expID,       /* exposure identifier                                  */
  char *obsetID,       /* observation set identifier                           */
  ...                  /* variable argument list                               */
) {

  /* check the inherited status */
  if ( STATUS_BAD(*istat) ) return;
  MNSN_CLEAR_STR(resp,'\0');
    
  /* check input parameters */
/*   if ( expID==(double *)NULL || obsetID==(char *)NULL ) { */
/*     *istat = DHS_ERROR; */
/*     MNSN_WRITE_VAL(resp,"ERROR","dhsIOCtl-E-bad parameter",*istat); */
/*     return; */
/*   } */

  /* check input values */
  if ( dhsID < (dhsHandle)0 ) {
    *istat = DHS_ERROR;
    MNSN_WRITE_VAL(resp,"ERROR","dhsIOCtl-E-bad value",*istat);
    return;
  }

  /* switch on ioctlFunction */
  switch (ioctlFunction) {
   case DHS_IOC_SETFILENAME:

   case DHS_IOC_OBSCONFIG:
   case DHS_IOC_MDCONFIG:
   case DHS_IOC_FPCONFIG:
   case DHS_IOC_KEYWORD_TRANS:
   case DHS_IOC_DEBUG_LVL:
   case DHS_IOC_SIMULATION:
     *istat = DHS_OK;
     MNSN_WRITE_VAL(resp,"OK","dhsIOCtl-I-succeeded",*istat);
     break;
   default:
    *istat = DHS_ERROR;
    MNSN_WRITE_VAL(resp,"ERROR","dhsIOCtl-E-bad ioctl",*istat);
  }

  /* return */
  return;
}
