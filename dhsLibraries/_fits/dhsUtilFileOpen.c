/*******************************************************************************
 *
 * __doc__ \subsection {dhsUtilFileOpen}
 * __doc__ \begin{description}
 * __doc__  \item[\sc use:] \emph{void dhsUtilFileOpen(long $\star$istat, char $\star$resp, double $\star$expID,
 * __doc__     fitsfile $\star$fd);}
 * __doc__  \item[\sc description:] Open or create the requested fits file
 * __doc__  \item[\sc argument(s):]
 * __doc__   \begin{description}
 * __doc__    \item[\ ] \ 
 * __doc__    \item[\emph{long $\star$istat}] the inherited status value.
 * __doc__    \item[\emph{char $\star$resp}] a user supplied buffer for message strings.
 * __doc__   \item[\emph{double $\star$expID}] exposure identifier.
 * __doc__   \item[\emph{fitsfile $\star$fd}] Address of the place holde for the file descriptor.
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
 * dhsUtilFileOpen ( ... )
 *******************************************************************************/ 
void dhsUtilFileOpen( long *istat,     /* inherited status                     */
		      char *resp,      /* response message                     */
		      double *expID,   /* exposure identifier                  */
		      fitsfile **fd
		      
    ) 
{
    /* declare some variable and initialize them */
    int fitsStatus = 0;
    char *fitsDir = (char *) NULL;
    char fitsFile[DHS_MAXSTR] = {'\0'};
    char tFName[DHS_MAXSTR] = {'\0'};
    struct stat fitsExists;
    int procDebug = 0;

    (void) memset((void *)&fitsExists,0,sizeof(struct stat));
    (void) memset((void *)&fitsFile[0],'\0',DHS_MAXSTR);
    switch (dhsUtilFLAG)
    {
      case 1:
	  fitsDir = ( (fitsDir=getenv("MONSOON_DATA")) == (char *) NULL ? (char *) "./"  : fitsDir );
	  if (fitsDir[strlen(fitsDir)-1] == '/') fitsDir[strlen(fitsDir)-1] ='\000';
	  (void) strncpy(dhsUtilDIR, fitsDir, DHS_MAXSTR); dhsUtilFLAG=3; 
	  (void) sprintf(tFName,"%s/%s",fitsDir,dhsUtilFILENAME);
	  break;
      case 2:
	  (void) sprintf(tFName,"%s/%f",dhsUtilDIR,*expID);
	  break; 
      case 3:
	  (void) sprintf(tFName,"%s/%s",dhsUtilDIR,dhsUtilFILENAME);
	  break;
      case 0: default:
      {
	  fitsDir = ( (fitsDir=getenv("MONSOON_DATA")) == (char *) NULL ? (char *) "./"  : fitsDir );
	  if (fitsDir[strlen(fitsDir)-1] == '/') fitsDir[strlen(fitsDir)-1] ='\000';
	
	  (void) sprintf(tFName,"%s/%f",fitsDir,*expID);
      }
    }

    DPRINTF(10, procDebug, "dhsUtilFileOpen: fitsDir=>%s<\n", dhsUtilDIR);
    DPRINTF(10, procDebug, "dhsUtilFileOpen: fileName=>%s<\n", dhsUtilFILENAME);
    DPRINTF(10, procDebug, "dhsUtilFileOpen: tFName=>%s<\n", tFName);

    /* check if the filename exists if it doesn't we'll create it*/
    sprintf(fitsFile,"%s.fits",tFName);
    (void) memset((void *)&fitsExists,0,sizeof(struct stat));
    if ( stat(fitsFile,&fitsExists)==-1 && errno==ENOENT ) 
    {
	if ( fits_create_file(fd,fitsFile,&fitsStatus) ) 
	{
	    *istat = DHS_ERROR;
	    sprintf(resp,"dhsUtilFileOpen: fits container file create failed, fitsStatus=%d.",fitsStatus);
	    MNSN_RET_FAILURE(resp,"");
	    return;
	}
    } 
    else /* file already exists if we did not create it we'll change the name and try again*/
    {
	if (dhsFileCreated == 1) /* we created the file for this exposure */
	{     /* just open it fd will be filled in by ffopen*/
	    if ( ffopen(fd,fitsFile,READWRITE,&fitsStatus) ) 
	    {	
		*istat = DHS_ERROR;
		sprintf(resp,"dhsUtilFileOpen: fits container file open failed, fitsStatus=%d.",fitsStatus);
		MNSN_RET_FAILURE(resp,"");
		return;
	    }
	    MNSN_RET_SUCCESS(resp,"dhsUtilFileOpen: Success.");
	    return;
	} 
	else
	{   /* the file exists from a previous exposure find a new name and create or open it */
	    while ( stat(fitsFile,&fitsExists)==-1 && errno==ENOENT)
	    {
		sprintf(fitsFile,"%s%c.fits",tFName,(char)dhsExtChar++);
	    }
	    if ( fits_create_file(fd,fitsFile,&fitsStatus) ) 
	    {
		*istat = DHS_ERROR;
		sprintf(resp,"dhsUtilFileOpen: fits container file create2 failed, fitsStatus=%d.",fitsStatus);
		MNSN_RET_FAILURE(resp,"");
		return;
	    }
	    MNSN_RET_SUCCESS(resp,"dhsUtilFileOpen: Success.");
	    return;	    
	}
	    
    }

    MNSN_RET_SUCCESS(resp,"dhsUtilFileOpen: Success.");
    return;	    
}
