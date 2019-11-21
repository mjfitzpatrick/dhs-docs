/*******************************************************************************
 * 
 * __doc__ \section {The dhsUtil <<VERSION>> Library}
 * __doc__ \subsection {dhsUtil.h}
 * __doc__ \begin{description}
 * __doc__  \item[\sc use:] \emph{\#include ``dhsUtil.h''}
 * __doc__  \item[\sc description:] this file contains all common code
 * __doc__    required by the functions needed to build the static
 * __doc__    and dynamic dhsUtil libraries. These libraries
 * __doc__    abstract the DHS interface to the system.
 * __doc__  \item[\sc argument(s):]  not applicable
 * __doc__  \item[\sc return(s):] not applicable
 * __doc__  \item[\sc last modified:] Monday, 4 November 2002
 * __doc__ \end{description} 
 *
 ******************************************************************************/

#ifndef _dhsImpl_H_
 #define _dhsImpl_H_ 1.0.1
 #define _dhsImpl_S_ "1.0.1"
 #define _dhsImpl_A_ "P. N. Daly, N. C. Buchholz"
 #define _dhsImpl_C_ "(c) AURA Inc, 2004. All rights reserved."

/*******************************************************************************
 * include(s) 
 ******************************************************************************/

/* #include
 * "/home/chisholm/QUANTA-0.4/include/QUANTA/QUANTAnet_remoteParallelFileIO32_c.hxx"
 * */

/******************************************************************************
 * define(s)
 ******************************************************************************/
 #define DHS_IMPLEMENTATION	"FITS"
 #define DHS_IMPL_MAXSTR         128 /* maximum length of an obsetID string  */

/******************************************************************************
 * typedef(s)
 ******************************************************************************/
/* typedef struct QUANTAnet_remoteParallelFileIOClient32_c
                QUANTAnet_remoteParallelFileIOClient32_c; */
 typedef struct dhsImpl_str {
    char   filename [DHS_MAXSTR];	/* default file name */
    char   directory[DHS_MAXSTR];	/* default directory name */
 } dhsImpl_t, *dhsImpl_p, **dhsImpl_s;

 typedef struct dhsCtrl_str {		/* structure to control the dhs Library holds default values */
    long   	dhsState;		/* state of the DHS */
    long   	simLevel;		/* >= 1 if in simulation mode */
    long	dhsDebug;		/* level of debug output from dhs */
    double 	expID;			/* default exposure ID  */
    char   	obs_ID  [DHS_MAXSTR];	/* default obs_ID string */
    char   	filename[DHS_MAXSTR];	/* default file name */
    fpConfig_t 	focalPlaneCfg;		/* default focal plane configuration */
    mdConfig_t 	metaDataCfg;		/* default metaData block */
    dhsImpl_p   implStruct;		/* pointer to any implementation specific data required */
 } dhsCtrl_t, *dhsCtrl_p, **dhsCtrl_s;
 
 struct dhsMem {
    long  processes;		/* number of process which have done an Open */		   
    ulong version;    
    dhsCtrl_t ctrl;
    long unused;
 };

/*******************************************************************************
 * prototype(s)
 ******************************************************************************/
#endif
