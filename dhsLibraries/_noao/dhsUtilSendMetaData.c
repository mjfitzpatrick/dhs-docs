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
#if !defined(_FITSIO_H)
 #include "fitsio.h"
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

  /* declare some variable and initialize them */
  int nr = 0;
  int fc = 0;
  int cpos = 0;
  int fitsStatus = 0;
  char *avpName[] = { "Name", "Value", "Comment" };
  char *avpFormat[] = { "A32", "A32", "A64" };
  char *avpUnits[] = { "string", "string", "string" };
  char avpCol1[DHS_AVP_NAMESIZE] = {'\0'};
  char avpCol2[DHS_AVP_VALSIZE] = {'\0'};
  char avpCol3[DHS_AVP_COMMENT] = {'\0'};
  char *avpRecord[4] = { avpCol1, avpCol2, avpCol3, (char *)NULL };
  char *fitsDir = (char *) NULL;
  fitsfile *fitsID = (fitsfile *)NULL;
  char fitsFile[DHS_MAXSTR] = {'\0'};
  char fitsRecord[DHS_FITS_RECLEN] = {'\0'};
  struct stat fitsExists;

  /* check the inherited status */
  if ( STATUS_BAD(*istat) ) return;
  MNSN_CLEAR_STR(resp,'\0');
    
  /* check input parameters */
  if ( cfg==(mdConfig_t *)NULL || expID==(double *)NULL) {
    *istat = DHS_ERROR;
    MNSN_WRITE_VAL(resp,"ERROR","dhsSendMetaData: cfg or expID is NULL. cfg=",(ulong) cfg);
    return;
  }

  /* check input values */
  if ( dhsID<(dhsHandle)0 || cfg->numFields>DHS_MAXFIELDS ) {
    *istat = DHS_ERROR;
    MNSN_WRITE_VAL(resp,"ERROR","dhsSendMetaData: dhs not open or too many fields. numfields=", cfg->numFields);
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
  (void) memset((void *)&fitsFile,'\0',DHS_MAXSTR);
  fitsDir = ( (fitsDir=getenv("MONSOON_DATA")) == (char *) NULL ? (char *) "./"  : fitsDir );
  if (fitsDir[strlen(fitsDir)-1] == '/') fitsDir[strlen(fitsDir)-1] ='\000';
  (void) sprintf(fitsFile,"%s/%f.fits",fitsDir,*expID);

  /* if it doesn't exist, create it */
  (void) memset((void *)&fitsExists,0,sizeof(struct stat));
  if ( stat(fitsFile,&fitsExists)==-1 && errno==ENOENT ) {
    if ( fits_create_file(&fitsID,fitsFile,&fitsStatus) ) {
      *istat = DHS_ERROR;
      MNSN_WRITE_VAL(resp,"ERROR","dhsSendMetaData-E-fits create container file failed",fitsStatus);
      return;
    }

  /* else file already exists, open it */
  } else {
    if ( ffopen(&fitsID,fitsFile,READWRITE,&fitsStatus) ) {
      *istat = DHS_ERROR;
      MNSN_WRITE_VAL(resp,"ERROR","dhsSendMetaData-E-fits open container file failed",fitsStatus);
      return;
    }
  }

  /* write headers here */
  switch (cfg->metaType) {

    /* FITS header records */
    case DHS_MDTYPE_FITSHEADER:
      nr = (int) blkSize / DHS_FITS_RAWLEN;
      for ( fc=0; fc<nr; fc++ ) {
	/* initialize record */
        cpos = 0;
        (void) memset((void *)&fitsRecord[cpos],'\0',(size_t)DHS_FITS_RECLEN);
	/* copy name and increment pointers/counters */
	(void) memmove((void *)&fitsRecord[cpos],blkAddr,(size_t)DHS_FITS_NAMESIZE);
       	blkAddr += DHS_FITS_NAMESIZE;
       	cpos += DHS_FITS_NAMESIZE;
	/* copy first separator and increment pointers/counters */
	(void) memmove((void *)&fitsRecord[cpos],"= ",(size_t)DHS_FITS_SEP1);
       	cpos += DHS_FITS_SEP1;
	/* copy value and increment pointers/counters */
	(void) memmove((void *)&fitsRecord[cpos],blkAddr,(size_t)DHS_FITS_VALSIZE);
       	blkAddr += DHS_FITS_VALSIZE;
       	cpos += DHS_FITS_VALSIZE;
	/* copy second separator and increment pointers/counters */
	(void) memmove((void *)&fitsRecord[cpos]," / ",(size_t)DHS_FITS_SEP2);
       	cpos += DHS_FITS_SEP2;
	/* copy comment and increment pointers/counters */
	(void) memmove((void *)&fitsRecord[cpos],blkAddr,(size_t)DHS_FITS_COMMENT);
       	blkAddr += DHS_FITS_COMMENT;
	/* now write out the record */
	if ( ffprec(fitsID,fitsRecord,&fitsStatus) ) {
          *istat = DHS_OK;
          MNSN_WRITE_VAL(resp,"WARN","dhsSendMetaData-W-fits failed at record",fc);
          MNSN_WRITE_VAL(resp,"WARN","dhsSendMetaData-W-fits record status",fitsStatus);
        }
      }
      break;

    /* AVP header records as FITS ASCII table */
    case DHS_MDTYPE_AVPAIR:
      nr = (int) blkSize / DHS_AVP_RAWLEN;
      /* create an ascii table */
      if ( obsetID==(char *)NULL || strlen(obsetID)<=(size_t)1 ) {
        ffcrtb(fitsID,ASCII_TBL,nr,cfg->numFields,(char **)&avpName,(char **)&avpFormat,(char **)&avpUnits,"PAN AV-Pairs List",&fitsStatus);
      } else {
        ffcrtb(fitsID,ASCII_TBL,nr,cfg->numFields,(char **)&avpName,(char **)&avpFormat,(char **)&avpUnits,obsetID,&fitsStatus);
      }
      if ( fitsStatus ) {
        *istat = DHS_ERROR;
        MNSN_WRITE_VAL(resp,"ERROR","dhsSendMetaData-E-fits ascii table create failed",fitsStatus);
        return;
      }
      (void) ffpkyj(fitsID,"EXTTYPE",cfg->metaType,"meta data type",&fitsStatus);
      /* populate the ascii table */
      for ( fc=0; fc<nr; fc++ ) {
	(void) memmove((void *)&avpCol1[0],blkAddr,(size_t)DHS_AVP_NAMESIZE); blkAddr += DHS_AVP_NAMESIZE;
        if ( ffpcls(fitsID,1,fc+1,1,1,(char **)&avpRecord[0],&fitsStatus) ) {
          *istat = DHS_ERROR;
          MNSN_WRITE_VAL(resp,"ERROR","dhsSendMetaData-E-fits asc1 table write failed",fitsStatus);
          return;
	}
	(void) memmove((void *)&avpCol2[0],blkAddr,(size_t)DHS_AVP_VALSIZE);  blkAddr += DHS_AVP_VALSIZE;
        if ( ffpcls(fitsID,2,fc+1,1,1,(char **)&avpRecord[1],&fitsStatus) ) {
          *istat = DHS_ERROR;
          MNSN_WRITE_VAL(resp,"ERROR","dhsSendMetaData-E-fits asc2 table write failed",fitsStatus);
          return;
	}
	(void) memmove((void *)&avpCol3[0],blkAddr,(size_t)DHS_AVP_COMMENT);  blkAddr += DHS_AVP_COMMENT;
        if ( ffpcls(fitsID,3,fc+1,1,1,(char **)&avpRecord[2],&fitsStatus) ) {
          *istat = DHS_ERROR;
          MNSN_WRITE_VAL(resp,"ERROR","dhsSendMetaData-E-fits asc3 table write failed",fitsStatus);
          return;
	}
      }
      break;

    case DHS_MDTYPE_SHIFTLIST:
      *istat = DHS_OK;
      MNSN_WRITE_VAL(resp,"OK","dhsSendMetaData-I-shiftlist not yet implemented",*istat);
      break;

    case DHS_MDTYPE_ARRAYDATA:
      *istat = DHS_OK;
      MNSN_WRITE_VAL(resp,"OK","dhsSendMetaData-I-arraydata not yet implemented",*istat);
      break;

    default:
      break;
  }

  /* flush and close */
  (void) ffflus(fitsID,&fitsStatus); fitsStatus = 0;
  if ( fits_close_file(fitsID,&fitsStatus) ) {
    *istat = DHS_ERROR;
    MNSN_WRITE_VAL(resp,"ERROR","dhsSendMetaData-E-fits close failed",fitsStatus);
    return;
  }

  /* return */
  return;
}
