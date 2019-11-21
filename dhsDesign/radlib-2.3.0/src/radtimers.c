/*---------------------------------------------------------------------------
 
  FILENAME:
        radtimers.c
 
  PURPOSE:
        Timer subsystem.
 
  REVISION HISTORY:
        Date            Engineer        Revision        Remarks
        5/26/95         M.S. Teel       0               original
        11/20/96        M.S. Teel       1               port to pSos ARM
        9/28/99         MS Teel         2               PORT TO Psos 2.50
        12/25/01        MS Teel         3               Port to linux
 
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

/*  ... OS include files
*/

/*  ... System include files
*/

/*  ... Local include files
*/
#include <radtimers.h>


/*  ... global memory declarations
*/

/*  ... static (local) memory declarations
*/
static TIMER_LIST_ID    timerList;




/*  ... subsystem internal calls
*/

/*  ... Tick off times (in signal handler context only!)
*/
static int timerProcess (void)
{
    register TIMER_ID   head,timer;
    ULONG               numberOfTicks;
    SYS_CALLBACK_MSG    cbMsg;
    int                 retVal;

    numberOfTicks = sysTimerGetTick () - timerList->lastTick;
    timerList->lastTick = sysTimerGetTick ();

    head = &timerList->head;

    for (timer = head->next;
            timer->deltaTime <= numberOfTicks;
            timer = head->next)
    {
        numberOfTicks -= timer->deltaTime;
        timer->pending = FALSE;
        head->next = timer->next;
        timer->next->prev = head;
        if (timer->routine != NULL)
        {
            cbMsg.length    = sizeof (cbMsg) - sizeof (cbMsg.length);
            cbMsg.msgType   = 0;
            cbMsg.callback  = timer->routine;
            cbMsg.parm      = timer->parm;
            retVal = write (timerList->notifyFD, &cbMsg, sizeof (cbMsg));
            if (retVal != sizeof (cbMsg))
            {
                radMsgLog (PRI_HIGH, "timerProcess: write to notify fd failed: %s",
                           strerror (errno));
            }
        }
    }

    if (timer != &timerList->tail)
    {
        timer->deltaTime -= numberOfTicks;
        return timer->deltaTime;
    }
    else
    {
        /*  ... no timers on list, return 0xFFFFFFFF for timeout (wait indefinitely)
        */
        return 0xFFFFFFFF;
    }
}

static void timerSignalHandler (int signum)
{
    /*  ... process the timer list here and restart based on the return value
    */
    radUtilsSetIntervalTimer (timerProcess ());

    return;
}


/*  ... API calls
*/
ULONG sysTimerGetTick (void)
{
    return radTimeGetMSSinceEpoch ();
}


/*  ... create a timer list
*/
int radTimerListCreate
(
    int                 noTimers,
    int                 notifyDescriptor
)
{
    TIMER_ID            timer;
    UCHAR               *memory;
    int                 i;
    struct sigaction    action;

    memory = (UCHAR *)
             malloc (sizeof (*timerList) + (sizeof (*timer) * noTimers));
    if (memory == NULL)
    {
        return ERROR;
    }

    timerList = (TIMER_LIST_ID) memory;
    memset (timerList, 0, sizeof (*timerList));

    /*  ... Set up the free list of timers
    */
    timerList->noFreeTimers = noTimers;
    timerList->notifyFD     = notifyDescriptor;
    timerList->freeList     = NULL;
    timer = (TIMER_ID) (timerList + 1);

    for (i = 0; i < noTimers; i ++)
    {
        timer->next = timerList->freeList;
        timerList->freeList = timer;
        timer += 1;
    }

    /*  ... initialize the delta list
    */
    timerList->head.next        = &timerList->tail;
    timerList->head.prev        = NULL;
    timerList->tail.next        = NULL;
    timerList->tail.prev        = &timerList->head;
    timerList->head.deltaTime   = 0L;
    timerList->tail.deltaTime   = 0xffffffffL;


    /*  ... catch SIGALRM
    */
    memset (&action, 0, sizeof (action));
    action.sa_handler = timerSignalHandler;
    if (sigemptyset (&action.sa_mask) == -1)
    {
        free (timerList);
        return ERROR;
    }

    if (sigaction (SIGALRM, &action, NULL) == -1)
    {
        free (timerList);
        return ERROR;
    }

    radUtilsEnableSignal (SIGALRM);
    return OK;
}


void radTimerListDelete
(
)
{
    radUtilsDisableSignal (SIGALRM);
    free (timerList);
}


/*  ... Allocate/modify a timer, returns NULL if none are available
*/
TIMER_ID radTimerCreate
(
    TIMER_ID        timer,
    void            (*routine) (void *parm),
    void            *parm
)
{
    radUtilsDisableSignal (SIGALRM);
    if (timer == NULL)
    {
        timer = timerList->freeList;
        if (timer == NULL)
        {
            radUtilsEnableSignal (SIGALRM);
            return NULL;
        }

        timerList->freeList = timer->next;
        timerList->noFreeTimers -= 1;
        timer->pending = FALSE;
    }

    timer->routine = routine;
    timer->parm = parm;

    radUtilsEnableSignal (SIGALRM);
    return (timer);
}


/*  ... Deallocate a timer
*/
void radTimerDelete
(
    TIMER_ID        timer
)
{
    if (timer == NULL)
        return;

    /*  ... If timer is on the delta list, remove it
    */
    radTimerStop (timer);

    radUtilsDisableSignal (SIGALRM);
    timer->next = timerList->freeList;
    timerList->freeList = timer;
    timerList->noFreeTimers += 1;
    radUtilsEnableSignal (SIGALRM);
    return;
}


/*  ... put a timer on the delta list (start it)
*/
void radTimerStart
(
    TIMER_ID        timer,
    int             time
)
{
    TIMER_ID        prev, next;
    int             resetTimer = FALSE;


    if (timer == NULL)
        return;

    if (timer->pending == TRUE)
    {
        radTimerStop (timer);
    }

    radUtilsDisableSignal (SIGALRM);

    timer->pending = TRUE;
    if (time == 0)
        time = 1;

    if (timerList->head.next == &timerList->tail)
    {
        /*      ... first timer to go on list - set the system tick
        */
        timerList->lastTick = sysTimerGetTick ();
        resetTimer = TRUE;
    }

    for (prev = &timerList->head,next = prev->next;
            next->deltaTime < time;
            prev = next, next = next->next)
    {
        time -= next->deltaTime;
    }

    if (prev == &timerList->head)
    {
        resetTimer = TRUE;
    }

    timer->prev = prev;
    timer->next = next;
    timer->deltaTime = time;
    prev->next = timer;
    next->prev = timer;
    if (next != &timerList->tail)
        next->deltaTime -= time;


    if (resetTimer)
    {
        radUtilsSetIntervalTimer (time);
    }

    radUtilsEnableSignal (SIGALRM);
    return;
}



void radTimerStop
(
    TIMER_ID        timer
)
{
    TIMER_ID       next, prev;


    if (timer == NULL)
        return;

    if (timer->pending == FALSE)
        return;

    radUtilsDisableSignal (SIGALRM);

    timer->pending = FALSE;

    next = timer->next;
    prev = timer->prev;
    if (next != &timerList->tail)
        next->deltaTime += timer->deltaTime;

    next->prev = prev;
    prev->next = next;

    radUtilsEnableSignal (SIGALRM);
}


int radTimerStatus
(
    TIMER_ID        timer
)
{
    if (timer == NULL)
        return (FALSE);

    return (timer->pending);
}


void radTimerSetUserParm
(
    TIMER_ID        timer,
    void            *newParm
)
{
    if (timer == NULL)
        return;

    radUtilsDisableSignal (SIGALRM);
    timer->parm = newParm;
    radUtilsEnableSignal (SIGALRM);
    return;
}

