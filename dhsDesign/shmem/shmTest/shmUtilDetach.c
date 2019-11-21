/*******************************************************************************
 *
 * __doc__ \subsection {shmUtilDetach}
 * __doc__ \begin{description}
 * __doc__  \item[\sc use:] \emph{void shmUtilDetach(long $\star$istat, char $\star$resp, char $\star$libName, void $\star$address );}
 * __doc__  \item[\sc description:] this function detaches from the
 * __doc__   shared memory segment for global library variables. A flag indicates that
 * __doc__   the memory should also be destroyed. The inherited status is updated and returned along with a message response.
 * __doc__  \item[\sc argument(s):]
 * __doc__   \begin{description}
 * __doc__    \item[\ ] \ 
 * __doc__    \item[\emph{long $\star$istat}] the inherited status value.
 * __doc__    \item[\emph{char $\star$resp}] a user supplied buffer for message strings.
 * __doc__    \item[\emph{char $\star$libName}] the library name.
 * __doc__    \item[\emph{void $\star$address}] the address of the segment to detach from.
 * __doc__   \end{description}
 * __doc__  \item[\sc return(s):] void
 * __doc__  \item[\sc last modified:] Thursday, 21 November 2002
 * __doc__  \item[\sc author(s):] Phil Daly (pnd), Nick Buchholz (ncb)
 * __doc__  \item[\sc license:] (c) 2002 AURA Inc. All rights reserved. Released under the GPL.
 * __doc__ \end{description}
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 ******************************************************************************/

/*******************************************************************************
 * include(s):
 *******************************************************************************/
#if !defined(_shmUtil_H_)
#include "shmUtil.h"
#endif

/*******************************************************************************
 * shmUtilDetach ( ... )
 *******************************************************************************/
void shmUtilDetach(long *istat,	/* inherited status                        */
		   char *resp,	/* response message                        */
		   char *libName,	/* the lock file name                      */
		   void *address	/* the address to detach from              */
    )
{
    /* declare some variables and initialize them */
    struct shmid_ds _detBuf;
    key_t memKey = (key_t) ERROR;
    long retVal = OK;
    int shmFlag = 0x00001B6;
    int new = FALSE, mfd = 0, size = 10;
    (void) memset((void *) &_detBuf, 0, sizeof(struct shmid_ds));

    /* check the inherited status */
    if (STATUS_BAD(*istat))
	return;
    MNSN_CLEAR_STR(resp, '\0');

    /* find the memory segment if shmid is 0 */
    if ((libName == (char *) NULL)) {
	*istat = ERROR;
	MNSN_RET_FAILURE(resp,
			 "shmDetach:  bad parameter, libName == NULL.");
	return;
    }

    /* get key */
    memKey = shmInit(istat, resp, libName, &new);
    if (STATUS_BAD(*istat)) {
	return;
    }

    if ((mfd = shmget((key_t) memKey, (int) size, (int) shmFlag)) == ERROR) {
	*istat = makeMSE(errno);
	sprintf(resp, "shmDetach: shmget call failed. \\\\%s.",
		strerror(errno));
	MNSN_RET_FAILURE(resp, "");
	return;
    }

    if ((retVal = (long) shmctl(mfd, IPC_STAT, &_detBuf)) != OK) {
	*istat = makeMSE(retVal);
	sprintf(resp, "shmDetach: shmctl call failed. \\\\%s.",
		strerror(errno));
	MNSN_RET_FAILURE(resp, "");
	return;
    }

    /* detach the shared memory segment */
    if ((retVal = (long) shmdt(address)) != OK) {
	*istat = makeMSE(retVal);
	sprintf(resp, "shmDetach: shmdt call failed. \\\\%s.",
		strerror(errno));
	MNSN_RET_FAILURE(resp, "");
    }

    if (_detBuf.shm_nattch == 1) {
	if ((retVal = (long) shmctl(mfd, IPC_RMID, &_detBuf)) != OK) {
	    *istat = makeMSE(retVal);
	    sprintf(resp, "shmDetach: shmctl call 2 failed, \\\\%s.",
		    strerror(errno));
	    MNSN_RET_FAILURE(resp, "");
	}
	*istat = OK;
	shmUninit(istat, resp, libName);
    }

    /* return */
    return;
}
