/*******************************************************************************
 *
 * __doc__ \subsection {dhsUtilSendPixelData}
 * __doc__ \begin{description}
 * __doc__  \item[\sc use:] \emph{void dhsUtilSendPixelData(long $\star$istat, char $\star$resp, 
 * __doc__    dhsHandle dhsID, void $\star$pxlAddr, size\_t pxlSize, fpConfig\_t $\star$cfg, 
 * __doc__    double $\star$expID, char $\star$obsetID);}
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
#if !defined(_FITSIO_H)
 #include "fitsio.h"
#endif

/*******************************************************************************
 * dhsUtilSendPixelData ( ... )
 *******************************************************************************/ 
void dhsUtilSendPixelData ( long *istat,     /* inherited status               */
			    char *resp,      /* response message               */
			    dhsHandle dhsID, /* dhs handle                     */
			    void *pxlAddr,   /* address of data block          */
			    size_t blkSize,  /* size of data block             */
			    fpConfig_t *cfg, /* configuration of pixel data    */
			    double *expID,   /* exposure identifier            */
			    char *obsetID    /* observation set identifier     */
    ) 
{
    /* declare some variable and initialize them */
    int fs=0;
    int bitpix=32;
    long nelms=0L;
    long naxis=2L;
    long naxes[2] = { 0L, 0L };
    fitsfile *fd = (fitsfile *)NULL;
    char *fitsDir = (char *) NULL;
    char fitsFile[DHS_MAXSTR] = {'\0'};
    struct stat sts;
    int procDebug = 0;


    (void) memset((void *)&sts,0,sizeof(struct stat));
    (void) memset((void *)&fitsFile[0],'\0',DHS_MAXSTR);

    /* check the inherited status */
    if ( STATUS_BAD(*istat) ) return;
    MNSN_CLEAR_STR(resp,'\0');
    
    /* check input parameters */
    if ( pxlAddr==(void *)NULL || expID==(double *)NULL) {
	*istat = DHS_ERROR;
	sprintf(resp,"dhsSendPixelData: Bad parameter. %s",
			 		((expID==(double *)NULL) ? "expID ptr==NULL." : "FP config ptr == NULL."));
	MNSN_RET_FAILURE(resp,"");
	return;
    }

    /* check input values */
    if ( dhsID<(dhsHandle)0 || blkSize<=(size_t)0) 
    {
	*istat = DHS_ERROR;
	sprintf(resp,"dhsSendPixelData: %s",((dhsID<(dhsHandle)0) ? "dhsHandle invalid" : "blkSize <= 0."));
	MNSN_RET_FAILURE(resp,"");
	return;
    }

    switch (dhsUtilFLAG)
    {
      case 1:
	  fitsDir = ( (fitsDir=getenv("MONSOON_DATA")) == (char *) NULL ? (char *) "./"  : fitsDir );
	  if (fitsDir[strlen(fitsDir)-1] == '/') fitsDir[strlen(fitsDir)-1] ='\000';
	  (void) strncpy(dhsUtilDIR, fitsDir, DHS_MAXSTR); dhsUtilFLAG=3; 
	  (void) sprintf(fitsFile,"%s/%s.fits",fitsDir,dhsUtilFILENAME);
	  break;
      case 2:
	  (void) sprintf(fitsFile,"%s/%f.fits",dhsUtilDIR,*expID);
	  break; 
      case 3:
	  (void) sprintf(fitsFile,"%s/%s.fits",dhsUtilDIR,dhsUtilFILENAME);
	  break;
      case 0: default:
      {
	  fitsDir = ( (fitsDir=getenv("MONSOON_DATA")) == (char *) NULL ? (char *) "./"  : fitsDir );
	  if (fitsDir[strlen(fitsDir)-1] == '/') fitsDir[strlen(fitsDir)-1] ='\000';
	
	  (void) sprintf(fitsFile,"%s/%f.fits",fitsDir,*expID);
      }
    }

    DPRINTF(10, procDebug, "dhsSendMetaData: fitsDir=>%s<\n", dhsUtilDIR);
    DPRINTF(10, procDebug, "dhsSendMetaData: fileName=>%s<\n", dhsUtilFILENAME);
    DPRINTF(10, procDebug, "dhsSendMetaData: fitsFile=>%s<\n", fitsFile);

    /* set output size */
    naxes[0] = cfg->xSize;
    naxes[1] = cfg->ySize;
    nelms = naxes[0] * naxes[1];

    /* if it doesn't exist, create it */
    if ( stat(fitsFile,&sts)==-1 && errno==ENOENT ) 
    {
	if ( fits_create_file(&fd,fitsFile,&fs) ) 
	{
	    *istat = DHS_ERROR;
	    sprintf(resp,"dhsSendPixelData:  fits container file create failed, fitsStatus = %d", fs);
	    MNSN_RET_FAILURE(resp,"");
	    return;
	}
    } 
    else /* else file already exists, open it */
    {
	if ( ffopen(&fd,fitsFile,READWRITE,&fs) ) 
	{
	    *istat = DHS_ERROR;
	    sprintf(resp,"dhsSendPixelData:  fits container file open failed, fitsStatus = %d", fs);
	    MNSN_RET_FAILURE(resp,"");
	    return;
	}
    }
  
    /* make it a FITS mage file */
    if ( fits_create_img(fd,bitpix,naxis,naxes,&fs) ) 
    {
	*istat = DHS_ERROR;
	sprintf(resp,"dhsSendPixelData:  fits image file create failed, fitsStatus = %d", fs);
	MNSN_RET_FAILURE(resp,"");
	(void) fits_close_file(fd,&fs);
	return;
    }

    /* write data as long integers */
    if ( fits_write_img(fd,TLONG,1L,nelms,pxlAddr,&fs) ) 
    {
	*istat = DHS_ERROR;
	sprintf(resp,"dhsSendPixelData:  fits write image file failed, fitsStatus = %d", fs);
	MNSN_RET_FAILURE(resp,"");
	(void) fits_close_file(fd,&fs);
	return;
    }

    /* flush and close */
    (void) ffflus(fd,&fs);
    if ( fits_close_file(fd,&fs) ) 
    {
	*istat = DHS_ERROR;
	 sprintf(resp,"dhsSendPixelData: fits close failed, fitsStatus = %d", fs);
	 MNSN_RET_FAILURE(resp,"");
	return;
    }

    /* return */
    MNSN_RET_SUCCESS(resp, "dhsSendPixelData: Success.");
    return;
}
