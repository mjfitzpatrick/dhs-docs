/*******************************************************************************
 *
 * __doc__ \subsection {dhsUtilSendPixelData}
 * __doc__ \begin{description}
 * __doc__  \item[\sc use:] \emph{void dhsUtilSendPixelData(long $\star$istat, char $\star$resp, dhsHandle dhsID, void $\star$pxlAddr, size\_t pxlSize, fpConfig\_t $\star$cfg, double $\star$expID, char $\star$obsetID);}
 * __doc__  \item[\sc description:] send pixel data to the DHS.
 * __doc__  \item[\sc argument(s):]
 * __doc__   \begin{description}
 * __doc__    \item[\ ] \ 
 * __doc__    \item[\emph{long $\star$istat}] the inherited status value.
 * __doc__    \item[\emph{char $\star$resp}] a user supplied buffer for message strings.
 * __doc__   \item[\emph{dhsHandle dhsID}] dhs handle.
 * __doc__   \item[\emph{void $\star$pxlAddr}] address of data block.
 * __doc__   \item[\emph{size\_t $\star$pxlSize}] size of data block.
 * __doc__   \item[\emph{fpConfig\_t $\star$cfg}] pixel data configuration.
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
 * dhsUtilSendPixelData ( ... )
 *******************************************************************************/ 
void dhsUtilSendPixelData (
  long *istat,     /* inherited status                                         */
  char *resp,      /* response message                                         */
  dhsHandle dhsID, /* dhs handle                                               */
  void *pxlAddr,   /* address of data block                                    */
  size_t pxlSize,  /* size of data block                                       */
  fpConfig_t *cfg, /* configuration of pixel data                              */
  double *expID,   /* exposure identifier                                      */
  char *obsetID    /* observation set identifier                               */
) {

  /* declare some variables and initialize them */
  int fd = 0;
  char *dataDir = (char *) NULL;
  char fname[DHS_MAXSTR] = {'\0'};
  struct stat sts;
  (void) memset((void *)&sts,0,sizeof(struct stat));
  (void) memset((void *)&fname[0],'\0',sizeof(DHS_MAXSTR));

  /* check the inherited status */
  if ( STATUS_BAD(*istat) ) return;
  MNSN_CLEAR_STR(resp,'\0');
    
  /* check input parameters */
  if ( pxlAddr==(void *)NULL || expID==(double *)NULL ) {
    *istat = DHS_ERROR;
    MNSN_WRITE_VAL(resp,"ERROR","dhsSendPixelData-E-bad parameter",*istat);
    return;
  }

  /* check input values */
  if ( dhsID<(dhsHandle)0 || pxlSize<=(size_t)0 || *expID<=(double)0.0 ) {
    *istat = DHS_ERROR;
    MNSN_WRITE_VAL(resp,"ERROR","dhsSendPixelData-E-bad value",*istat);
    return;
  }

  /* create the file name of the form /MNSN/soft_dev/data/2453039.123456.fits */
  dataDir = ( (dataDir=getenv("MONSOON_DATA")) == (char *) NULL ? (char *) "./"  : dataDir );
  if (dataDir[strlen(dataDir)-1] == '/') dataDir[strlen(dataDir)-1] ='\000';
  (void) sprintf(fname,"%s/%f.fits",dataDir,*expID);

  /* if it doesn't exist, create it */
  if ( stat(fname,&sts)==-1 && errno==ENOENT ) {
    if ( (fd=creat(fname,S_IRWXU|S_IRWXG|S_IRWXO)) < 0 ) {
      *istat = DHS_ERROR;
      MNSN_WRITE_VAL(resp,"ERROR","dhsSendPixelData-E-creat container file failed",*istat);
      return;
    }
  }

  /* for this library, this is a no-op */
  *istat = DHS_OK;
  MNSN_WRITE_VAL(resp,"OK","dhsSendPixelData-I-succeeded",*istat);

  /* return */
  return;
}
