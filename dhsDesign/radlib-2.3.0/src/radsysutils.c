/*---------------------------------------------------------------------------
 
  FILENAME:
        radsysutils.c
 
  PURPOSE:
        Provide the system utilities.
 
  REVISION HISTORY:
        Date            Engineer        Revision        Remarks
        12/24/01        M.S. Teel       0               Original
 
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

/*  ... System include files
*/

/*  ... Library include files
*/

/*  ... Local include files
*/
#include <radsysutils.h>


/*  ... global memory declarations
*/

/*  ... global memory referenced
*/

/*  ... static (local) memory declarations
*/


/* ... methods
*/

int radUtilsSetIntervalTimer (int msecs)
{
    struct itimerval    tval;

    memset (&tval, 0, sizeof (tval));
    tval.it_value.tv_sec  = msecs/1000;
    tval.it_value.tv_usec = (msecs%1000) * 1000;

    return (setitimer (ITIMER_REAL, &tval, NULL));
}


int radUtilsGetIntervalTimer (void)
{
    struct itimerval    tval;
    int                 retVal;

    memset (&tval, 0, sizeof (tval));
    if (getitimer (ITIMER_REAL, &tval) == -1)
    {
        return ERROR;
    }

    retVal = tval.it_value.tv_sec * 1000;
    retVal += (tval.it_value.tv_usec / 1000);

    return retVal;
}


int radUtilsEnableSignal (int signum)
{
    sigset_t    sigset;

    if (sigemptyset (&sigset) == -1)
    {
        return ERROR;
    }

    if (sigaddset (&sigset, signum) == -1)
    {
        return ERROR;
    }

    if (sigprocmask (SIG_UNBLOCK, &sigset, NULL) == -1)
    {
        return ERROR;
    }

    return OK;
}


int radUtilsDisableSignal (int signum)
{
    sigset_t    sigset;

    if (sigemptyset (&sigset) == -1)
    {
        return ERROR;
    }

    if (sigaddset (&sigset, signum) == -1)
    {
        return ERROR;
    }

    if (sigprocmask (SIG_BLOCK, &sigset, NULL) == -1)
    {
        return ERROR;
    }

    return OK;
}


int radUtilsBecomeDaemon (const char *workingDirectory)
{
    pid_t       newPid;

    newPid = fork ();
    if (newPid < 0)
    {
        return ERROR;
    }
    else if (newPid != 0)
    {
        /*  ... we are the parent, bail
        */
        exit (0);
    }

    /*  ... become new session/group leader
    */
    setsid ();

    /*  ... change working directory
    */
    if (workingDirectory)
    {
        chdir (workingDirectory);
    }
    else
    {
        chdir ("/");
    }

    /*  ... clear the file mode mask
    */
    umask (0);

    return (getpid ());
}


/*  ... provide a sleep function with ms resolution;
*/
void radUtilsSleep (int msDuration)
{
    struct timespec     timespec;

    timespec.tv_sec = msDuration/1000;
    timespec.tv_nsec = (msDuration%1000)*1000000;

    nanosleep (&timespec, NULL);

    return;
}
