/*******************************************************************************
 *
 * __doc__ \subsection {dhsUtilAux}
 * __doc__ \begin{description}
 * __doc__  \item[\sc use:] \emph{Not Applicable}
 * __doc__  \item[\sc description:] This module declares a single global variable
 * __doc__   a shared memory structure pointer. This variable is used to contain the 
 * __doc__   pointer to the shared memory used by the library routines for global info
 * __doc__  \item[\sc argument(s):] None
 * __doc__  \item[\sc return(s):] None
 * __doc__  \item[\sc last modified:] Wednesday, 21 January 2003
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
extern int dhsDebug;
#define DEBUG	1
#include <debug.h>

#if !defined(_dhsUtil_H_)
 #include "dhsUtil.h"
#endif

#if !defined(_dhsImpl_H_)
 #include "dhsImplementationSpecifics.h"
#endif 

dhsImpl_t impStruct;

struct dhsMem *dhsMemP;

