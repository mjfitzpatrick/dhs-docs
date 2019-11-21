/*******************************************************************************
 *
 * __doc__ \subsection {shmUtilInit}
 * __doc__ \begin{description}
 * __doc__  \item[\sc use:] \emph{key\_t shmUtilInit(long $\star$istat, char $\star$resp, char $\star$libName, int $\star$create);}
 * __doc__  \item[\sc description:] this function initializes the shared memory segment file lock.
 * __doc__   The inherited status is updated.
 * __doc__  \item[\sc argument(s):]
 * __doc__   \begin{description}
 * __doc__    \item[\ ] \ 
 * __doc__    \item[\emph{long $\star$istat}] the inherited status value.
 * __doc__    \item[\emph{char $\star$resp}] a user supplied buffer for message strings.
 * __doc__    \item[\emph{char $\star$libName}] the library name.
 * __doc__    \item[\emph{int $\star$create}] TRUE if we need to create the segment first (returns shmid).
 * __doc__   \end{description}
 * __doc__  \item[\sc return(s):] a key\_t related to the lock file to be used in creating the shared memory
 * __doc__  \item[\sc last modified:] Monday, 12 March 2003
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
 * shmUtilInit ( ... )
 ******************************************************************************/
key_t shmUtilInit(long *istat,	/* inherited status                        */
		  char *resp,	/* response message                        */
		  char *libName,	/* returned channel identifier             */
		  int *new	/* new segment flag                        */
    )
{
    /* declare some variables and initialize them */
    char fname[MAXMSG] = { '\0' };
    FILE *lockFile = (FILE *) NULL;
    int posit = 0;
    key_t memKey = (key_t) ERROR;

    /* set new before checking status */
    *new = FALSE;

    /* check the inherited status */
    if (STATUS_BAD(*istat))
	return memKey;
    MNSN_CLEAR_STR(resp, '\0');

    /* check input parameters */
    if (libName == (char *) NULL) {
	*istat = ERROR;
	MNSN_RET_FAILURE(resp,
			 "shmInit:  bad parameter, libName == NULL.");
	return memKey;
    }

    /* get the lock file name and pid */
    (void) memset((void *) &fname[0], '\0', MAXMSG);
    (void) sprintf(fname, "/tmp/%s.LCK", libName);

    /* try to open it */
    if ((lockFile = fopen(fname, "a")) == (FILE *) NULL) {
	*istat = makeMSE(errno);
	sprintf(resp, "shmInit:  fopen call failed for file %s.\\\\%s",
		fname, strerror(errno));
	MNSN_RET_FAILURE(resp, "");
	return memKey;
    }

    /* check if it is a new file or not */
    if ((posit = (int) ftell((FILE *) lockFile)) == 0) {
	*new = TRUE;
    }
    (void) fprintf(lockFile, "%ld:\t", (long) getpid());
    fflush(lockFile);
    (void) fclose(lockFile);

    /* get a token */
    if ((memKey = ftok(fname, (strlen(fname) & 0xFF))) == ERROR) {
	*istat = makeMSE(errno);
	sprintf(resp, "shmInit:  ftok call failed.\\\\%s",
		strerror(errno));
	MNSN_RET_FAILURE(resp, "");
	return memKey;
    }

    /* return */
    if (ALLOK(*istat)) {
	MNSN_RET_SUCCESS(resp, "shmInit: Success.");
    }
    return memKey;
}
