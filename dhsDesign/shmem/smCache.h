/*
 *  SMCACHE -- Shared Memory Cache Interface.
 *
 */



/*
**  Defined Constants
*/

#define SMC_DEBUG	1
#define	SMC_CACHE_FILE	"/tmp/.smc"
#define SMC_LOCKFILE    "/tmp/.smc.LCK"

#define SZ_PATH		164
#define SZ_FNAME	 64


#ifndef TRUE
#define TRUE            1
#endif
#ifndef FALSE
#define FALSE           0
#endif
#ifndef OK
#define OK              0
#endif
#ifndef ERR
#define ERR             -1
#endif


/*
**  Globals
*/

static long sys_pagesize;



/*
** Data Structures
*/


typedef struct smCache 	 smCache_t;
typedef struct smcPage 	 smcPage_t;

typedef struct smSegment smSegment_t;


/*  The main shared memory cache structure.  The SMC is basically a shared
**  memory structure used to manage "pages" of shared memory data.  This is
**  the one public entry point into the cache, clients needing a 'page' for 
**  data will be returned a pointer to the page segment to be used by the
**  API for loading data and header elements.  The cache manages the
**  allocation of these pages and will recycle an allocated page of the 
**  same size if available or else create a new shared memory segment.
**
**  Page data is maintained as a doubly linked list of smcPage structures
**  within the data area of the cache.  Each entry in the cache contains
**  only the information needed to access another shared memory object of
**  the proper size and type, called a 'Segment'.
**
**  Pages are marked as free when all registered clients have detached from
**  the page and marked it as "done" (TBD).  The API will allow a client to
**  prune the free pages in order to release space back to the system.
**/
struct smCache {
    ulong	shmID;		/* shared mem id			*/

    pthread_mutex_t lock;	/* mutex lock for the Cache		*/

    int    	cache_size;	/* Size of the entire cache (bytes) 	*/
    int    	page_size;	/* Size of an entry in cache 		*/

    int		np_used;	/* # of pages in use			*/
    int		np_free;	/* # of (allocated) free pages		*/
    int    	np_max;		/* max pages we can manage		*/
    int		np_hiwater;	/* High water mark of cache (npages)	*/

    ulong	mem_allocated;	/* sum of allocated page sizes (bytes)  */
    ulong	mem_avail;	/* sum of available page sizes (bytes)  */

    smcPage_t 	*pages;		/* Pointer to the 1st segment page	*/
};



/* smcPage structure definition.  A "Page" is a shared memory segment that
** is comprised of a descriptive header and an arbitrarily long userdata
** area.  Pages are allocated in units of the system page size and so may
** have padding at the end.  The start of the user-data is word aligned.
**/
struct smcPage {
    key_t  	shmid;          /* Segment shared mem id key            */
    int    	type;           /* Segment type (META, DATA, OTHER)     */
    int    	free;           /* Segment free flag?			*/
    ulong  	size;           /* Size of user data (bytes)            */
    ulong  	time;           /* Epoch page was created		*/

    smcPage_t   *next, *prev;	/* Linked list pointers			*/
};



/* smSegment sructure definition.
**
**/
struct smSegment {
    smCache_t   *cache;		/* back ptr to the cache		*/
    smcPage_t   *page_entry;	/* back ptr to entry int the cache	*/

    key_t  	shmid;          /* Segment shared mem id key            */
    int    	type;           /* Segment type (META, DATA, OTHER)     */
    int    	free;           /* Segment free flag?			*/
    ulong  	size;           /* Size of ser data (bytes)             */
    ulong  	time;           /* Epoch page was created		*/

    int    	locked;         /* Is Segment locked for access?        */
    pthread_mutex_t lock;	/* mutex lock for the Cache		*/

    int    	nattched;       /* No. of attached processes            */
    int    	refcount;       /* reference count                      */

    ulong  	who;            /* who sent the data                    */
    double 	expID;          /* exposure ID                          */
    char   	*obsetID;       /* observation set ID                   */

/*  mdConfig_t	mdConfig;	// metadata configuration struct	*/
/*  fpConfig_t	fpConfig;	// focal plane configuration struct	*/

    union {			/* User data area			*/
        long 	l;
        double  d;
        void    *p;
    } userdata[1];
};


/*
** Utility Macros
*/

/* Mapping isn't backed by any file.  MAP_ANON is deprecated but aliased
 * here for systems that don't have MAP_ANONYMOUS.
 */
#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS MAP_ANON
#endif

#define align(s,a) (((s)+(a)-1) & ~((a)-1))


/*
**  Prototypes
*/

/* shmUtil.c */
key_t  shmUtilInit   (int *new, char *lockfile);
void  *shmUtilAttach (key_t memKey, ulong size, int *create);
void   shmUtilDetach (key_t memKey, void *address, int clearLock);
ulong  shmUtilSize   (ulong *inSize, ulong inUnit);
int    shmUtilExists (key_t memKey, int mode);



