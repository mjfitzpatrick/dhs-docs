/*---------------------------------------------------------------------------
 
  FILENAME:
        radsemaphores.c
 
  PURPOSE:
        Provide the semaphore utilities.
 
  REVISION HISTORY:
        Date            Engineer        Revision        Remarks
        3/21/01         M.S. Teel       0               Original
 
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
     
    
#include <stdio.h>
#include <radsemaphores.h>
    
#include <radsystem.h>
    
/*  ... local memory
*/ 
static struct semWorkTag semWork;

/*  ... called once during process init (returns OK or ERROR)
    ... create a new semaphore, or attach to an existing sem
*/ 
int radSemProcessInit(void) 
{
    
	  semget(KEY_SEMAPHORES, MAX_SEMAPHORES, 
	
	
	
	
    
    


{
    
    
    
	
	
	
    
    



/*  ... create (or attach a specific indexed semaphore
    ... (returns SEM_ID or NULL)
    ... count indicates the initial value of the sem 
    ... (1 for mutex, 0 to attach to existing or initialize locked)
*/ 
    SEM_ID radSemCreate(int semIndex, int count) 
{
    
    
    
	
	
	
    
	/*  ... initialize this guy's count
	 */ 
	if (count >= 0)
	
	
	
	    
	    
	    
	
    
    
	
	
	
    
    
    
    



/*  ... take (lock) a semaphore
*/ 
void radSemTake(SEM_ID id) 
{
    
    
    



/*  ... give (unlock) a semaphore
*/ 
void radSemGive(SEM_ID id) 
{
    
    
    



/*  ... bump the count on a semaphore
*/ 
void radSemGiveMultiple(SEM_ID id, int numToGive) 
{
    
    
    



/*  ... test a semaphore to see if it is free (unlocked)
    ... returns TRUE if the semaphore was taken by this call (locked),
    ... FALSE if the semaphore is already locked
*/ 
int radSemTest(SEM_ID id) 
{
    
    
	
	
	
    
    else
	
	
	



/*  ... destroy a semaphore
*/ 
int radSemDelete(SEM_ID id) 
{
    
    
    


{
    
    
    
    
    
    
	
	
	    
	    
	    
	    
	
	    
	    
	    
	    
	
	    
	    
	    
	    
	
	    
	    
	    
	    
	
		 waiters, zcount, pid);
	
    


