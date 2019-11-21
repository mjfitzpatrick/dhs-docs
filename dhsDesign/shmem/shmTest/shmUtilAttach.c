/*******************************************************************************
 *
 * __doc__ \subsection {shmUtilAttach}
 * __doc__ \begin{description}
 * __doc__  \item[\sc use:] \emph{void $\star$shmUtilAttach(long $\star$istat, char $\star$resp, char $\star$libName, int $\star$create, int size);}
 * __doc__  \item[\sc description:] this function initializes the shared memory segment.
 * __doc__   The inherited status is updated.
 * __doc__  \item[\sc argument(s):]
 * __doc__   \begin{description}
 * __doc__    \item[\ ] \ 
 * __doc__    \item[\emph{long $\star$istat}] the inherited status value.
 * __doc__    \item[\emph{char $\star$resp}] a user supplied buffer for message strings.
 * __doc__    \item[\emph{char $\star$libName}] the library name.
 * __doc__    \item[\emph{int $\star$create}] TRUE if we need to create the segment first (returns shmid).
 * __doc__    \item[\emph{int size}] the size of the desired segment.
 * __doc__   \end{description}
 * __doc__  \item[\sc return(s):] pointer to memory address or (void $\star$) NULL.
 * __doc__  \item[\sc last modified:] Wednesday, 12 March 2003
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
 * shmUtilAttach ( ... )
 ******************************************************************************/
void *shmUtilAttach(long *istat,	/* inherited status                     */
		    char *resp,	/* response message                     */
		    char *libName,	/* returned channel identifier          */
		    int *create,	/* create segment flag                  */
		    long size	/* size of buffer                       */
    )
{
    /* declare some variables and initialize them */
    int new = FALSE;
    key_t memKey = (key_t) 0;
    int mFd = 0;
    int shmFlag = 0x00001B6;
    void *retAddr = (void *) NULL;
    char fname[MAXMSG] = { '\0' };
    FILE *lockFile = (FILE *) NULL;

    /* check the inherited status */
    if (STATUS_BAD(*istat))
	return retAddr;
    MNSN_CLEAR_STR(resp, '\0');

    /* check parameters */
    if (libName == (char *) NULL || create == (int *) NULL || size == 0L) {
	*istat = INVALID_ARG;
	sprintf(resp, "shmUtilAttach: bad parameter, %s.",
		((libName == (char *) NULL) ? "libName==NULL" :
		 ((size == 0L) ? "size==0" : "create==NULL")));
	MNSN_RET_FAILURE(resp, "");
	return retAddr;
    }

    (void) memset((void *) &fname[0], '\0', MAXMSG);
    (void) sprintf(fname, "/tmp/%s.LCK", libName);
    if ((memKey = shmInit(istat, resp, libName, (int *) &new)) == ERROR) {
	*istat = ERROR;
	return retAddr;
    }

    /* make sure size is right */
    (void) shmSize(istat, (char *) NULL, (unsigned long *) &size,
		   sizeof(long));

    /* get the shared memory segment */
    if (*create)
	shmFlag |= IPC_CREAT;

    if ((mFd = shmget((key_t) memKey, (int) size, (int) shmFlag)) == ERROR) {
	*istat = ERROR;
	sprintf(resp, "shmUtilAttach: shmget failed.\\\\%s.",
		strerror(errno));
	MNSN_RET_FAILURE(resp, "");
	return retAddr;
    }

    /* attach to shared memory */
    if ((retAddr = (void *) shmat(mFd, 0, 0)) == (void *) ERROR) {
	*istat = makeMSE(errno);
	sprintf(resp, "shmUtilAttach: shmat failed.\\\\%s.",
		strerror(errno));
	MNSN_RET_FAILURE(resp, "");
	return (void *) NULL;
    }

    if ((lockFile = fopen(fname, "a")) == (FILE *) NULL) {
	*istat = makeMSE(errno);
	sprintf(resp, "shmAttach:  lockfile fopen failed.\\\\%s.",
		strerror(errno));
	MNSN_RET_FAILURE(resp, "");
	return (void *) NULL;
    } else {
	(void) fprintf(lockFile, "%ld, %ld\n", (long) mFd, (long) size);
	fflush(lockFile);
	(void) fclose(lockFile);
    }


    /* make sure the first long word of the memory is zeroed */
    if (new)
	(void) memset((void *) retAddr, 0, size);

    /* return */
    *create = mFd;
    *istat = OK;
    return ((void *) retAddr);
}
