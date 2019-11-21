/*******************************************************************************
 *
 * __doc__ \subsection {dhsUtilSendMetaData}
 * __doc__ \begin{description}
 * __doc__  \item[\sc use:] \emph{void dhsUtilSendMetaData(long $\star$istat, char $\star$resp, dhsHandle dhsID, void $\star$blkAddr, size\_t blkSize, mdConfig\_t $\star$cfg, double $\star$expID, char $\star$obsetID);}
 * __doc__  \item[\sc description:] send meta data to the DHS.
 * __doc__  \item[\sc argument(s):]
 * __doc__   \begin{description}
 * __doc__    \item[\ ] \ 
 * __doc__    \item[\emph{long $\star$istat}] the inherited status value.
 * __doc__    \item[\emph{char $\star$resp}] a user supplied buffer for message strings.
 * __doc__   \item[\emph{dhsHandle dhsID}] dhs handle.
 * __doc__   \item[\emph{void $\star$blkAddr}] address of data block.
 * __doc__   \item[\emph{size\_t $\star$blkSize}] size of data block.
 * __doc__   \item[\emph{mdConfig\_t $\star$cfg}] meta data configuration.
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
 * dhsUtilSendMetaData ( ... )
 *******************************************************************************/ 
void dhsUtilSendMetaData (
  long *istat,     /* inherited status                                         */
  char *resp,      /* response message                                         */
  dhsHandle dhsID, /* dhs handle                                               */
  void *blkAddr,   /* address of data block                                    */
  size_t blkSize,  /* size of data block                                       */
  mdConfig_t *cfg, /* configuration of meta data                               */
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
  if ( cfg==(mdConfig_t *)NULL || expID==(double *)NULL ) {
    *istat = DHS_ERROR;
    MNSN_WRITE_VAL(resp,"ERROR","dhsSendMetaData-E-bad pointer",*istat);
    return;
  }

  /* check input values */
  if ( dhsID<(dhsHandle)0 || *expID<=(double)0.0 || cfg->numFields>DHS_MAXFIELDS ) {
    *istat = DHS_ERROR;
    MNSN_WRITE_VAL(resp,"ERROR","dhsSendMetaData-E-bad value",*istat);
    return;
  }

  /* check address/size */
  if ( blkAddr==(void *)NULL && blkSize!=(size_t)0 ) {
    *istat = DHS_ERROR;
    MNSN_WRITE_VAL(resp,"ERROR","dhsSendMetaData-E-bad input",*istat);
    return;
  }

  /* return OK if size is 0 */
  if ( blkSize == (size_t)0 ) {
    *istat = DHS_OK;
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
      MNSN_WRITE_VAL(resp,"ERROR","dhsSendMetaData-E-creat container file failed",*istat);
      return;
    }
  }

  /* for this library, this is a no-op */
  *istat = DHS_OK;
  MNSN_WRITE_VAL(resp,"OK","dhsSendMetaData-I-succeeded",*istat);

  /* return */
  return;
}
