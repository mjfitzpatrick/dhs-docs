/*******************************************************************************
 *
 * __doc__ \subsection {shmUtilUninit}
 * __doc__ \begin{description}
 * __doc__  \item[\sc use:] \emph{void shmUtilUninit(long $\star$istat, char $\star$resp, char $\star$libName);}
 * __doc__  \item[\sc description:] this function un-initializes the shared memory segment.
 * __doc__   The inherited status is updated.
 * __doc__  \item[\sc argument(s):]
 * __doc__   \begin{description}
 * __doc__    \item[\ ] \ 
 * __doc__    \item[\emph{long $\star$istat}] the inherited status value.
 * __doc__    \item[\emph{char $\star$resp}] a user supplied buffer for message strings.
 * __doc__    \item[\emph{char $\star$libName}] the library name.
 * __doc__   \end{description}
 * __doc__  \item[\sc return(s):] void.
 * __doc__  \item[\sc last modified:] Monday, 4 November 2002
 * __doc__  \item[\sc author(s):] Nick Buchholz (ncb), Phil Daly (pnd)
 * __doc__  \item[\sc license:] (c) 2002 AURA Inc. All rights reserved, Released under the GPL.
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
 ******************************************************************************/
#if !defined(_shmUtil_H_)
#include "shmUtil.h"
#endif

/*******************************************************************************
 * shmUtilUninit ( ... )
 ******************************************************************************/
void shmUtilUninit(long *istat,	/* inherited status                        */
		   char *resp,	/* response message                        */
		   char *libName	/* returned channel identifier             */
    )
{

    /* declare some variables and initialize them */
    char fname[MAXMSG] = { '\0' };
    int lockFile = 0;

    /* check the inherited status */
    if (STATUS_BAD(*istat))
	return;
    MNSN_CLEAR_STR(resp, '\0');

    /* check inputs */
    if (libName == (char *) NULL) {
	*istat = ERROR;
	MNSN_RET_FAILURE(resp, "shmUninit:  bad parameter, libName==NULL");
	return;
    }

    /* get the lock file name and pid */
    (void) memset((void *) &fname[0], '\0', MAXMSG);
    (void) sprintf(fname, "/tmp/%s.LCK", libName);

    /* try to unlink it */
    if ((lockFile = unlink(fname)) == ERROR) {
	*istat = makeMSE(errno);
	sprintf(resp, "shmUninit:  unlink failed.\\\\%s", strerror(errno));
	MNSN_RET_FAILURE(resp, "");
    }

    /* return normally */
    if (ALLOK(*istat)) {
	MNSN_RET_SUCCESS(resp, "shmUninit: Success.");
    }
    return;
}
