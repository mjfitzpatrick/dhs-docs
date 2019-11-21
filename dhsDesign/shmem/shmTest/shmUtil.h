/*******************************************************************************
 *	
 * __doc__ \section {The shmUtil <<VERSION>> Library}
 * __doc__ \subsection {shmUtil.h}
 * __doc__ \begin{description}
 * __doc__  \item[\sc use:] \emph{\#include ``shmUtil.h''}
 * __doc__  \item[\sc description:] this file contains all common code
 * __doc__    required by the functions needed to build the static
 * __doc__    and dynamic shmUtil libraries. These libraries
 * __doc__    abstract the shared memory interface to the system.
 * __doc__  \item[\sc argument(s):]  not applicable
 * __doc__  \item[\sc return(s):] not applicable
 * __doc__  \item[\sc last modified:] Monday, 4 November 2002
 * __doc__ \end{description} 
 *
 ******************************************************************************/

#if !defined(_shmUtil_H_)
 #define _shmUtil_H_ 1.0.0
 #define _shmUtil_S_ "1.0.0"

 /*******************************************************************************
  * include(s)
  ******************************************************************************/
 #if !defined(_STDIO_H)
  #include <stdio.h>
 #endif
 
 #if !defined(_STDLIB_H)
  #include <stdlib.h>
 #endif
 
 #if !defined(_UNISTD_H)
  #include <unistd.h>
 #endif
 
 #if !defined(_SYS_TYPES_H)
  #include <sys/types.h>
 #endif
 
 #if !defined(_STRING_H)
  #include <string.h>
 #endif
 
 #if !defined(_CTYPE_H)
  #include <ctype.h>
 #endif
 
 #if !defined(_SYS_IPC_H)
  #include <sys/ipc.h>
 #endif
 
 #if !defined(_SYS_SHM_H)
  #include <sys/shm.h>
 #endif
 
 #if !defined(_SYS_STAT_H)
  #include <sys/stat.h>
 #endif
 
 #if !defined(_MNSN_STD_HDR_)
  #include <mnsnStd.h>
 #endif
 
 #if !defined(__MNSNERR__)
  #include <mnsnErrNo.h>
 #endif
 
 #if !defined(_DEBUG_H)
  #include <debug.h>
 #endif
 
extern char resp2[];
 /*******************************************************************************
  * define(s)
  ******************************************************************************/
 #define shmInit            shmUtilInit
 #define shmUninit          shmUtilUninit
 #define shmSize            shmUtilSize
 #define shmDetach          shmUtilDetach 
 #define shmAttach          shmUtilAttach 
 
 #define _utilShrdLibInit   shmUtilInit
 #define _utilShrdLibUninit shmUtilUninit
 #define _utilMemSize       shmUtilSize
 #define _utilDetach        shmUtilDetach 
 #define _utilAttach        shmUtilAttach 
 
 #define SHM_LOCKFILE "/tmp/libshmUtil.LCK"
 #define DIVIDES(a,n) (!((n) % (a)))
 
 /*******************************************************************************
  * prototype(s)
  ******************************************************************************/
 void * shmUtilAttach ( long *istat, char *resp, char *libName, int *create, long size );
 void   shmUtilDetach ( long *istat, char *resp, char *libName, void *address );
 key_t  shmUtilInit   ( long *istat, char *resp, char *libName, int *new );
 void   shmUtilUninit ( long *istat, char *resp, char *libName );
 ulong  shmUtilSize   ( long *istat, char *resp, ulong *inSize, ulong inUnit );
#else
#endif
