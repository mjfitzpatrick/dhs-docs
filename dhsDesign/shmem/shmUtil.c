/*
 *  SHMUTIL -- Shared memory cache utility functions.
 *
 *	    key = shmUtilInit  (new, lockfile)
 *	 addr = shmUtilAttach  (memKey, size, create)
 *		shmUtilDetach  (memKey, addr, clearLock)
 *	   size = shmUtilSize  (inSize, inUnit)
 *	 bool = shmUtilExists  (memKey, mode)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <errno.h>

#include "smCache.h"



/*  SHMUTILINIT -- Initialize a shared memory segment.
 */

key_t 
shmUtilInit (new, lockfile)
int 	*new;				/* new segment flag 		*/
char 	*lockfile;			/* returned lockfile name 	*/
{
	int posit 	= 0;
	key_t memKey 	= (key_t) ERR;
	FILE *lockFile 	= (FILE *) NULL;
	char fname[SZ_FNAME];


	*new = FALSE; 			/* set new before checking status */

	/* Get the lock file name and pid and try to open it.
	 */
	strcpy (fname, SMC_LOCKFILE);
	if ((lockFile = fopen (fname, "a")) == (FILE *) NULL) {
	    printf ("shmInit:  fopen call failed for file %s. '%s'",
		fname, strerror(errno));
	    lockfile[0] = (char)NULL;
	    return memKey;
	}

	/* Check whether it is a new file.
	 */
	if ((posit = (int) ftell ((FILE *) lockFile)) == 0)
	    *new = TRUE;

	fprintf (lockFile, "%ld:\t", (long) getpid());
	fflush (lockFile);
	fclose (lockFile);

	/* Get a token.
	 */
	if ((memKey = ftok (fname, (strlen(fname) & 0xFF))) == ERR)
	    printf ("shmInit:  ftok call failed. '%s'", strerror(errno));

	/* Copy the lockfile name to the output string.
	 */
	strncpy (lockfile, fname, strlen (fname));
	return memKey;
}


/*  SHMUTILATTACH --  Attach the current process to the shared memory
 *  segment identified by the key, or create it of the specified size
 *  if necessary.  If the 'new' flag is set the memory 
 */

void *
shmUtilAttach (memKey, size, create)
key_t 	memKey;				/* shared memory key		*/
ulong 	size;				/* size of buffer               */
int 	*create;			/* create segment flag?         */
{
	int mFd 	= 0;
	int shmFlag 	= 0x00001B6;	/* mode = 666			*/
	void *retAddr 	= (void *) NULL;
	FILE *lockFile 	= (FILE *) NULL;


	/* Check input parameters.
	 */
	if (memKey == (key_t) NULL || create == (int *) NULL || size == 0L) {
	    printf ("shmUtilAttach: bad parameter, %s.",
		((memKey == (key_t) NULL) ? "memKey==NULL" :
		    ((size == 0L) ? "size==0" : "create==NULL")));
	    return retAddr;
	}

	/* Get the shared memory segment.
	 */
	if (*create)
	    shmFlag |= IPC_CREAT;
	if ((mFd = shmget((key_t) memKey, size, shmFlag)) == ERR) {
	    printf ("shmUtilAttach: shmget failed. '%s'.", strerror(errno));
	    return retAddr;
	}


	/* Attach to shared memory
	 */
	if ((retAddr = (void *) shmat(mFd, 0, 0)) == (void *) ERR) {
	    printf ("shmUtilAttach: shmat failed. '%s'.", strerror(errno));
	    return (void *) NULL;
	}


	/* Update the lockfile.
	 */
	if ((lockFile = fopen (SMC_LOCKFILE, "a")) == (FILE *) NULL) {
	    printf ("shmAttach:  lockfile fopen failed. '%s'.", 
		strerror(errno));
	    return (void *) NULL;

	} else {
	    fprintf (lockFile, "%ld, %ld\n", (long) mFd, (long) size);
	    fflush (lockFile);
	    fclose (lockFile);
	}


	/* Clear the first long word of the memory if we're creating the
 	 * segment.
	 */
	if (*create)
	    memset((void *) retAddr, 0, size);

	/* Save the sm identifier in the 'create' flag and return the
	 * pointer to the space.
	 */
	*create = mFd;
	return ((void *) retAddr);
}


/*  SHMUTILDETACH --  Detach the current process from the segment
 *  specified by the memKey.  Clear the lock file and completely free
 *  the segment if we're the last process using it.
 */

void 
shmUtilDetach (memKey, address, clearLock)
key_t 	memKey;				/* shared memory segment key	*/
void 	*address;			/* the address to detach from	*/
int	clearLock;			/* clear lockfile if last proc? */
{
	struct shmid_ds _detBuf;
	long retVal 	= OK;
	int shmFlag 	= 0x00001B6;	/* mode = 666			*/
	int mFd 	= 0, 
	    lockFile 	= -1,
	    size 	= 10;


	/* Clear the buffer. */
	(void) memset((void *) &_detBuf, 0, sizeof(struct shmid_ds));


	/* Get the identifier for the segment belonging to the key.
	 */
	if ((mFd = shmget ((key_t) memKey, size, shmFlag)) == ERR) {
	    printf ("shmDetach: shmget call failed. '%s'.", strerror(errno));
	    return;
	}

	if ((retVal = (long) shmctl (mFd, IPC_STAT, &_detBuf)) != OK) {
	    printf ("shmDetach: shmctl call failed. '%s'.", strerror(errno));
	    return;
	}

	/* Detach the shared memory segment.
	 */
	if ((retVal = (long) shmdt (address)) != OK)
	    printf ("shmDetach: shmdt call failed. '%s'.", strerror(errno));

	/* If we're the only process left using it, delete it.
	 */
	if (_detBuf.shm_nattch == 1) {
	    if ((retVal = (long) shmctl (mFd, IPC_RMID, &_detBuf)) != OK) {
	        printf ("shmDetach: shmctl call 2 failed, '%s'.",
		    strerror(errno));
	    }

	    if (clearLock) {
	        if ((lockFile = unlink (SMC_LOCKFILE)) == ERR)
	            printf ("shmDetach: unlink failed. '%s'", strerror(errno));
	    }
	}
}


/*  SHMUTILSIZE -- Re-size a segment into an even multiple of some unit.
 */

ulong 
shmUtilSize (inSize, inUnit)
ulong  	*inSize;			/* requested size 		*/
ulong  	inUnit;				/* size of base unit 		*/
{
	/* Check input parameters.
	 */
	if (inSize == (ulong *) NULL || inUnit == 0L) {
	    printf ("shmSize:  bad parameter, %s",
		((inUnit == 0L) ? "inUnit==0L" : "inSize==NULL"));
	    return 0L;
	}

	/* Re-size if it doesn't divide evenly. 
	 */
	if (inUnit % (*inSize))
	    *inSize = ((*inSize / inUnit + 1L) * inUnit);

	return *inSize;
}


/*  SHMUTILEXISTS -- Determine whether the key points to an existing segment.
 *  accessible with the mode specified.
 */

int 
shmUtilExists (memKey, mode)
key_t 	memKey;				/* shared memory segment key	*/
int	mode;				/* access mode			*/
{
	key_t  smID = (key_t) NULL;
       
        return ((smID = shmget (memKey, 0, mode)) != -1);
}
