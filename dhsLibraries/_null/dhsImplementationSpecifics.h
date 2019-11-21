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
 #define _dhsImpl_H_ 1.0.0
 #define _dhsImpl_S_ "1.0.0"
 #define _dhsImpl_A_ "P. N. Daly, N. C. Buchholz"
 #define _dhsImpl_C_ "(c) AURA Inc, 2004. All rights reserved."

/*******************************************************************************
 * include(s) 
 ******************************************************************************/

/******************************************************************************
 * define(s)
 ******************************************************************************/
 #define DHS_IMPLEMENTATION	"NULL"
 #define DHS_IMPL_MAXSTR         128 /* maximum length of an obsetID string  */

/******************************************************************************
 * typedef(s)
 ******************************************************************************/
 typedef struct dhsImpl_str {
    char   filename[DHS_IMPL_MAXSTR];	/* default file name */
    char   directory[DHS_IMPL_MAXSTR];	/* default directory name */
 } dhsImpl_t, *dhsImpl_p, **dhsImpl_s;

/*******************************************************************************
 * prototype(s)
 ******************************************************************************/
#endif
