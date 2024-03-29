/*---------------------------------------------------------------------------
 
  FILENAME:
        radsysdefs.c
 
  PURPOSE:
        Provide system definitions.
 
  REVISION HISTORY:
        Date            Engineer        Revision        Remarks
        02/03/2004      MST             0               Original
 
  NOTES:
 
 
  LICENSE:
        Copyright 2001-2005 Mark S. Teel. All rights reserved.

        Redistribution and use in source and binary forms, with or without 
        modification, are permitted provided that the following conditions 
        are met:

        1. Redistributions of source code must retain the above copyright 
           notice, this list of conditions and the following disclaimer.
        2. Redistributions in binary form must reproduce the above copyright 
           notice, this list of conditions and the following disclaimer in the 
           documentation and/or other materials provided with the distribution.

        THIS SOFTWARE IS PROVIDED BY Mark Teel ``AS IS'' AND ANY EXPRESS OR 
        IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
        WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
        DISCLAIMED. IN NO EVENT SHALL MARK TEEL OR CONTRIBUTORS BE LIABLE FOR 
        ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
        DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
        OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
        HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
        STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
        IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
        POSSIBILITY OF SUCH DAMAGE.
  
----------------------------------------------------------------------------*/

/*  ... System header files
*/

/*  ... Local header files
*/
#include <radsysdefs.h>



/*  ... define the system buffer numbers here
*/
int sysBufferCounts[] =
{
    64,         /* size = 64 */
    128,        /* size = 128 */
    256,        /* size = 256 */
    256,        /* size = 512 */
    128,        /* size = 1024 */
    64,         /* size = 2048 */
    32,         /* size = 4096 */
    0
};


