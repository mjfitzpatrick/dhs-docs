/*******************************************************************************
 *
 * __doc__ \subsection {dhsUtilCloseExp}
 * __doc__ \begin{description}
 * __doc__  \item[\sc use:] \emph{void dhsUtilCloseExp(long $\star$istat, char $\star$resp, dhsHandle dhsID, double expID);}
 * __doc__  \item[\sc description:] close an exposure connection to the DHS.
 * __doc__  \item[\sc argument(s):]
 * __doc__   \begin{description}
 * __doc__    \item[\ ] \ 
 * __doc__    \item[\emph{long $\star$istat}] the inherited status value.
 * __doc__    \item[\emph{char $\star$resp}] a user supplied buffer for message strings.
 * __doc__   \item[\emph{dhsHandle dhsID}] dhs handle.
 * __doc__   \item[\emph{double expID}] exposure identifier.
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
#if !defined(_DEBUG_H)
 #include "debug.h"
#endif
#include "unistd.h"

/*******************************************************************************
 * dhsUtilCloseExp ( ... )
 *******************************************************************************/ 
void dhsUtilCloseExp (
  long *istat,     /* inherited status                                         */
  char *resp,      /* response message                                         */
  dhsHandle dhsID, /* dhs handle                                               */
  double expID     /* exposure identifier                                      */
) {
  /* declare some variable and initialize them */
  char *dataDir = (char *) NULL;
  char *rhost = (char *) NULL;
  char fitsFile[DHS_MAXSTR] = {'\0'};
  char image_list[DHS_MAXSTR];
  char *date = (char *) NULL;
  FILE *fp;

  /* check the inherited status */
  if ( STATUS_BAD(*istat) ) return;
  MNSN_CLEAR_STR(resp,'\0');
  *istat = DHS_OK;
    
  /* check input values */
  if ( dhsID<(dhsHandle)0 || expID<=(double)0.0 ) {
    *istat = DHS_ERROR;
    MNSN_WRITE_VAL(resp,"ERROR","dhsCloseExp-E-bad value",*istat);
    return;
  }

  /* for this library, this is a no-op */
  /* *istat = DHS_OK;
     MNSN_WRITE_VAL(resp,"OK","dhsCloseExp-I-succeeded",*istat); */

  /* using expID and MONSOON_DATA create file name for the current file
     in the form dataDir/expID.fits, i.e., /MNSN/soft_dev/data/2453039.123456.fits */

  dataDir = ( (dataDir=getenv("MONSOON_DATA")) == (char *) NULL ? (char *) "./"  : dataDir
);
  if (dataDir[strlen(dataDir)-1] == '/') dataDir[strlen(dataDir)-1] ='\000';
  (void) sprintf(fitsFile,"%s/%f.fits",dataDir,expID);


  /* DPRINT(0,10,"the value of rhost is ...\n");*/
  /* send fits file to remote host - hardwired */
  rhost = getenv("MONSOON_DATA_RHOST");
  /* client(rhost,fitsFile,fitsFile); */

/*   if ( execl("/MNSN/soft_dev/src/Util/dhsLibraries/_Gregs/client",
             "/MNSN/soft_dev/src/Util/dhsLibraries/_Gregs/client",rhost,fitsFile,fitsFile,0)) {
     *istat = DHS_ERROR;
     MNSN_WRITE_VAL(resp,"ERROR","execl use of Quanta failed",*istat);
     return;
        } */
  if (fork() == 0) {
    execl("/MNSN/soft_dev/src/Util/dhsLibraries/_Gregs/client",
          "client",rhost,fitsFile,fitsFile,0); }

  date = getenv("MONSOON_UTC");
  (void) sprintf(image_list,"%s/image_list.%s",dataDir,date);
  fp = fopen(image_list, "a+");
  if (fp != NULL) {
    fprintf(fp,"%f.fits\n",expID);
    fclose(fp);
  }

  if (fork() == 0) {
    execl("/MNSN/soft_dev/src/Util/dhsLibraries/_Gregs/client",
          "client",rhost,image_list,image_list,0); }

  /* return */
  return;
}
