/*******************************************************************************
 *
 * __doc__ \subsection {shmUtilSize}
 * __doc__ \begin{description}
 * __doc__  \item[\sc use:] \emph{u\_long shmUtilSize(long $\star$istat, char $\star$resp, u\_long $\star$inSize, u\_long inUnit);}
 * __doc__  \item[\sc description:] this function resizes inSize so that it is an exact multiple of
 * __doc__   of inUnit. It is used to ensure boundary integrity prior to memory allocation.
 * __doc__   The inherited status is updated.
 * __doc__  \item[\sc argument(s):]
 * __doc__   \begin{description}
 * __doc__    \item[\ ] \ 
 * __doc__    \item[\emph{long $\star$istat}] the inherited status value.
 * __doc__    \item[\emph{char $\star$resp}] a user supplied buffer for message strings.
 * __doc__    \item[\emph{u\_long $\star$inSize}] input size (modified on return).
 * __doc__    \item[\emph{u\_long inUnit}] the base unit for re-sizing.
 * __doc__   \end{description}
 * __doc__  \item[\sc return(s):] number of elements in new inSize.
 * __doc__  \item[\sc last modified:] Monday, 4 November 2002
 * __doc__  \item[\sc author(s):] Phil Daly (pnd)
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
 * shmUtilSize ( ... )
 *******************************************************************************/
ulong shmUtilSize(long *istat,	/* inherited status                        */
		  char *resp,	/* response buffer                         */
		  ulong * inSize,	/* requested size                           */
		  ulong inUnit	/* size of base unit                        */
    )
{

    /* check the inherited status */
    if (STATUS_BAD(*istat))
	return *inSize;
    MNSN_CLEAR_STR(resp, '\0');

    /* check input parameters */
    if (inSize == (ulong *) NULL || inUnit == 0L) {
	*istat = ERROR;
	sprintf(resp, "shmSize:  bad parameter, %s",
		((inUnit == 0L) ? "inUnit==0L" : "inSize==NULL"));
	MNSN_RET_FAILURE(resp, "");
	return 0L;
    }

    /* re-size */
    if (!DIVIDES(inUnit, *inSize))
	*inSize = ((*inSize / inUnit + 1L) * inUnit);

    /* return value */
    if (ALLOK(*istat)) {
	MNSN_RET_SUCCESS(resp, "shmSize: Success.");
    }
    return *inSize;
}
