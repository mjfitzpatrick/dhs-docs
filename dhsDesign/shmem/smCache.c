/*
 *  SMCACHE -- Shared Memory Cache Interface.
 *
 *
 *	       smc = smcOpen (npages, flags)
 *	       smcInitialize (smc)
 *	            smcClose (smc, free_flag)
 *        pages = smcFindNew (smc, client_code, type)
 *        page = smcFindPage (smc, shmid
 *	            smcPrune (smc)
 *
 *	    smp = smcGetPage (smc, type, size, attach_flag, lock_flag)
 *	         smcFreePage (smp)
 *
 *	           smcAttach (smp)
 *	           smcDetach (smp)
 *	             smcLock (smp)
 *	           smcUnlock (smp)
 *
 *     data = smcGetPageData (smp)
 *
 *       md = smcGetMDConfig (smp)
 *            smcSetMDConfig (smp, mdConfig)
 *       fp = smcGetFPConfig (smp)
 *            smcSetFPConfig (smp, fpConfig)
 *
 * Callbacks:
 *
 *       id = smcAddCallback (smc, callback_type, fcn, client_data)
 *         smcRemoveCallback (smc, id)
 *
 *             read_callback (client_data)
 *            write_callback (client_data)
 *
 *	   
 *  Example:
 *
 *  main (int argc, char **argv) {
 *	// Open a cache managing 64 pages.
 *	smc = smcOpen (64, NULL);
 *
 *	// Get a page with a 10K user data allocation, attach, don't lock
 *	page = smcGetPage (smc, TY_DATA, 10240, TRUE, FALSE)
 *
 *	memset (page->data, 1, 10240);    // fill the data area with a '1'
 *
 *	// Close the cache, detach but don't free.  Another client should
 *	// be able to connect to it to see the data. 
 *  	smcClose (smc, FALSE);
 *    }
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <stddef.h>

#include "smCache.h"




/*  SMCOPEN --  Open the Shared Memory Cache for the current process.  If
 *  the cache already exists we simply attach to it and return the structre
 *  pointer for the current process, otherwise we create it.  If the 'config'
 *  parameter is NULL we use hardwired defaults for the cache, otherwise it
 *  may point to a filename containing the configuration of the cache on this
 *  machine, or it can be a whitespace-delimited string of kw=value pairs
 *  specifying some or all of the configuration options.
 *
 *      Configuration options currently include:
 *
 *	cache_file	- no of "pages" to be managed
 *	npages		- no of "pages" to be managed
 *	lock_cache	- specifies whether cache is locked in memory
 *	lock_page	- specifies whether page segment are locked in memory
 */
smCache_t *
smcOpen  (char *config)
{
    int    	create = 0, new = 0, lock_cache = 0, lock_page = 0, npages = 0;
    ulong   	cache_size = 4096;
    void  	*cache_addr = NULL;
    pid_t  	pid;
    key_t  	memKey = (key_t) NULL;
    char  	cache_path[SZ_PATH];
    smCache_t	*smc = (smCache_t *) NULL;

    	
    /* Process the config options.
     */
    if (config == (char *) NULL) {
        sprintf (cache_path,  "%s%d", SMC_CACHE_FILE, (int) getpid());
    } else {
	printf ("Shouldn't be here, config params not yet implemented.....\n");
	exit (1);
    }


    /*  Open and/or initialize the SMC area.
     */
    memKey = shmUtilInit (&new, cache_path);
    if (!memKey) {
	fprintf (stderr, "Error initializing cache: '%s'\n", cache_path);
	return (smc);
    }

    /*  Attach to the segment.
     */
    cache_addr = (void *) shmUtilAttach (memKey, cache_size, &create);
    if (!cache_addr) {
        fprintf (stderr, "Error attaching to cache: file='%s' key=%ld\n", 
	    cache_path, (long) memKey);
	return (smc);
    }


    /* Debug output.
     */
    if (SMC_DEBUG) {
        printf ("smcOpen: init: memKey=%ld  file='%s' new=%d\n",
            (long) memKey, cache_path, new);
	printf ("smcOpen: attach: addr=%ld  create=%ld\n", 
	    (long) cache_addr, (long) create);
    }


    return (smc);
}


/*  SMCINITIALIZE -- Initialize the shared memory cache.  If the cache exists
 *  we free all the allocated resources and restore an initial state.
 */
smcInitialize  (smCache_t *smc)
{
}


/*
 */
void
smcClose  (smCache_t *smc, int free_flag)
{
}


/*
 */
smcPage_t **
smcFindNew (smCache_t *smc, ulong time, int type)
{
}


/*
 */
smcPrune  (smCache_t *smc)
{
}


/*
 */
smcPage_t *
smcGetPage (smCache_t *smc, int type, long size, int attach_flag, int lock_flag)
{
}


/*
 */
smcFreePage (smp)
{
}


/*
 */
smcAttach (smp)
{
}


/*
 */
smcDetach (smp)
{
}


/*
 */
smcLock (smp)
{
}


/*
 */
smcUnlock (smp)
{
}


/*
 */
void *
smcGetPageData (smp)
{
}


/*
mdConfig *
smcGetMDConfig (smp)
{
}
 */


/*
 */
smcSetMDConfig (smp, mdConfig)
{
}


/*
fpConfig *
p = smcGetFPConfig (smp)
{
}
 */


/*
 */
smcSetFPConfig (smp, fpConfig)
{
}
