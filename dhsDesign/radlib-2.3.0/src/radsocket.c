/*---------------------------------------------------------------------------
 
  FILENAME:
        radsocket.c
 
  PURPOSE:
        Provide the AF_INET socket utilities.
 
  REVISION HISTORY:
        Date            Engineer        Revision        Remarks
        06/05/2005      M.S. Teel       0               Original
 
  NOTES:
        This utility makes connections and passes data. Byte order or 
        content of the data is not considered. The user is responsible for 
        data contents and byte ordering conversions (if required).
 
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
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

/*  ... Library include files
*/
#include <radsocket.h>

/*  ... Local include files
*/

/*  ... global memory declarations
*/

/*  ... global memory referenced
*/

/*  ... static (local) memory declarations
*/


/* ... methods
*/

RADSOCK_ID radSocketServerCreate (int port)
{
    RADSOCK_ID          newId;
    int                 temp;
    struct sockaddr_in  sadrs;

    // first get our new object
    newId = (RADSOCK_ID) malloc (sizeof (*newId));
    if (newId == NULL)
    {
        return NULL;
    }
    memset (newId, 0, sizeof (*newId));
    newId->portno = port;

    // create our listen socket
    newId->sockfd = socket (PF_INET, SOCK_STREAM, 0);
    if (newId->sockfd == -1)
    {
        free (newId);
        return NULL;
    }

    // allow the port to be re-used in case we die and the socket lingers...
    temp = 1;
    if (setsockopt (newId->sockfd, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof (temp))
            == -1)
    {
        close (newId->sockfd);
        free (newId);
        return NULL;
    }

    // bind the socket descriptor to our address
    sadrs.sin_family        = AF_INET;
    sadrs.sin_port          = htons(newId->portno);
    sadrs.sin_addr.s_addr   = htonl(INADDR_ANY);
    memset (sadrs.sin_zero, 0, 8);

    if (bind (newId->sockfd, (struct sockaddr *)&sadrs, sizeof (sadrs)) == -1)
    {
        close (newId->sockfd);
        free (newId);
        return NULL;
    }

    // set the queue size of pending connections
    if (listen (newId->sockfd, 10) == -1)
    {
        close (newId->sockfd);
        free (newId);
        return NULL;
    }

    return newId;
}

RADSOCK_ID radSocketServerAcceptConnection (RADSOCK_ID id)
{
    socklen_t           adrsLen = sizeof (struct sockaddr_in);
    int                 retVal;
    UINT                tempIP;
    struct sockaddr_in  newAddr;
    RADSOCK_ID          newId;

    // first get our new object
    newId = (RADSOCK_ID) malloc (sizeof (*newId));
    if (newId == NULL)
    {
        return NULL;
    }
    memset (newId, 0, sizeof (*newId));

    if ((newId->sockfd = accept (id->sockfd, 
                                 (struct sockaddr *)&newAddr, 
                                 &adrsLen)) 
        == -1)
    {
        // connection error
        free (newId);
        return NULL;
    }

    newId->portno = ntohs(newAddr.sin_port);
    tempIP = ntohl(newAddr.sin_addr.s_addr);
    sprintf (newId->host, 
             "%d.%d.%d.%d",
             (tempIP >> 24) & 0xFF,
             (tempIP >> 16) & 0xFF,
             (tempIP >> 8) & 0xFF,
             (tempIP) & 0xFF);
             

    // turn off the transmit algorithm
    retVal = 1;
    if (setsockopt (newId->sockfd, 6, TCP_NODELAY, &retVal, sizeof (retVal)) == -1)
    {
        shutdown (newId->sockfd, 2);
        close (newId->sockfd);
        free (newId);
        return NULL;
    }

    return newId;
}

/* ... Create a socket client that connects to "hostNameOrIP" on  port "port";
   ... returns RADSOCK_ID or NULL if ERROR
*/
RADSOCK_ID radSocketClientCreate (char *hostNameOrIP, int port)
{
    RADSOCK_ID          newId;
    int                 temp, error;
    socklen_t           length = sizeof (struct sockaddr_in);
	socklen_t		    errlen;
    struct sockaddr_in  sadrs;
    struct hostent      *hostentptr = NULL;
    fd_set              wr, rd;
    int                 fdMax, retVal;
    struct timeval      tv;


    // first get our new object
    newId = (RADSOCK_ID) malloc (sizeof (*newId));
    if (newId == NULL)
    {
        return NULL;
    }
    memset (newId, 0, sizeof (*newId));
    strncpy (newId->host, hostNameOrIP, RADSOCK_MAX_HOST_LENGTH);

    // create our client socket
    newId->sockfd = socket (PF_INET, SOCK_STREAM, 0);
    if (newId->sockfd == -1)
    {
        radMsgLog (PRI_HIGH, "radSocketClientCreate: socket failed: %s", strerror(errno));
        free (newId);
        return NULL;
    }

    // initialize socket data structure
    memset (&sadrs, 0, sizeof(struct sockaddr_in));
    sadrs.sin_family = AF_INET;
    
    hostentptr = gethostbyname (newId->host);
    if (hostentptr == NULL)
    {
        radMsgLog (PRI_HIGH, "radSocketClientCreate: gethostbyname failed: %s", strerror(errno));
        close (newId->sockfd);
        free (newId);
        return NULL;
    }

    // get host address
    sadrs.sin_addr = *((struct in_addr *)hostentptr->h_addr);
    sadrs.sin_port = htons((USHORT)port);

    // set to non-blocking for the duration of the connect
    if (radSocketSetBlocking (newId, FALSE) == ERROR)
    {
        radMsgLog (PRI_HIGH, "radSocketClientCreate: radSocketSetBlocking failed: %s", strerror(errno));
        close (newId->sockfd);
        free (newId);
        return NULL;
    }
    
    // connect to remote host
    if (connect (newId->sockfd, (struct sockaddr *)&sadrs, sizeof(sadrs)) < 0)
    {
        if (errno == EINPROGRESS)
        {
            // wait until it wakes up on write, then continue
            FD_ZERO(&wr);
            FD_SET(newId->sockfd, &wr);
            rd = wr;
            fdMax = newId->sockfd;
            tv.tv_sec   = 3;
            tv.tv_usec  = 0;
            retVal = select (fdMax+1, &rd, &wr, NULL, &tv);
            if (retVal == 0)
            {
                radMsgLog (PRI_HIGH, "radSocketClientCreate: connect timeout");
                close (newId->sockfd);
                free (newId);
                return NULL;
            }
            else if (retVal < 0)
            {
                radMsgLog (PRI_HIGH, "radSocketClientCreate: select error");
                close (newId->sockfd);
                free (newId);
                return NULL;
            }
            else if (FD_ISSET(newId->sockfd, &rd) || FD_ISSET(newId->sockfd, &wr))
            {
                errlen = sizeof (error);
                if (getsockopt (newId->sockfd, SOL_SOCKET, SO_ERROR, &error, &errlen) < 0)
                {
                    radMsgLog (PRI_HIGH, "radSocketClientCreate: select getsockopt error");
                    close (newId->sockfd);
                    free (newId);
                    return NULL;
                }
            }
        }
        else
        {
            radMsgLog (PRI_HIGH, "radSocketClientCreate: connect failed: %s", strerror(errno));
            close (newId->sockfd);
            free (newId);
            return NULL;
        }
    }

    // set back to blocking
    if (radSocketSetBlocking (newId, TRUE) == ERROR)
    {
        radMsgLog (PRI_HIGH, "radSocketClientCreate: radSocketSetBlocking2 failed: %s", strerror(errno));
        shutdown (newId->sockfd, 2);
        close (newId->sockfd);
        free (newId);
        return NULL;
    }
    
    // get the local port number
    if (getsockname (newId->sockfd, (struct sockaddr *)&sadrs, &length) != 0)
    {
        radMsgLog (PRI_HIGH, "radSocketClientCreate: getsockname failed: %s", strerror(errno));
        shutdown (newId->sockfd, 2);
        close (newId->sockfd);
        free (newId);
        return NULL;
    }
    newId->portno = ntohs(sadrs.sin_port);

    
    // turn off the transmit algorithm
    temp = 1;
    if (setsockopt (newId->sockfd, 6, TCP_NODELAY, &temp, sizeof (temp)) == -1)
    {
        radMsgLog (PRI_HIGH, "radSocketClientCreate: setsockopt TCP_NODELAY failed: %s", strerror(errno));
        shutdown (newId->sockfd, 2);
        close (newId->sockfd);
        free (newId);
        return NULL;
    }

    // allow the port to be reused in case we die and the socket lingers...
    temp = 1;
    if (setsockopt (newId->sockfd, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof (temp))
        == -1)
    {
        radMsgLog (PRI_HIGH, "radSocketClientCreate: setsockopt SO_REUSEADDR failed: %s", strerror(errno));
        shutdown (newId->sockfd, 2);
        close (newId->sockfd);
        free (newId);
        return NULL;
    }

    // enable KEEPALIVE so we know when the far end goes away
    temp = 1;
    if (setsockopt (newId->sockfd, SOL_SOCKET, SO_KEEPALIVE, &temp, sizeof (temp))
        == -1)
    {
        radMsgLog (PRI_HIGH, "radSocketClientCreate: setsockopt SO_KEEPALIVE failed: %s", strerror(errno));
        shutdown (newId->sockfd, 2);
        close (newId->sockfd);
        free (newId);
        return NULL;
    }

    return newId;
}


/* ... Close connection and cleanup resources;
 ... returns OK or ERROR
*/
int radSocketDestroy (RADSOCK_ID id)
{
    shutdown (id->sockfd, 2);
    close (id->sockfd);
    free (id);
    return OK;
}

int radSocketGetDescriptor (RADSOCK_ID id)
{
    return id->sockfd;
}

char *radSocketGetHost (RADSOCK_ID id)
{
    return id->host;
}

int radSocketGetPort (RADSOCK_ID id)
{
    return (int)id->portno;
}

int radSocketReadExact
(
    RADSOCK_ID      id,
    void            *buffer,
    int             lenToRead
)
{
    UCHAR           *bPtr = (UCHAR *)buffer;
    int             retVal, bytesRead = 0;

    while (bytesRead < lenToRead)
    {
        retVal = read (id->sockfd, bPtr+bytesRead, lenToRead-bytesRead);
        if (retVal < 0)
        {
            /*  ... read error
            */
            if (errno == EAGAIN || errno == EINTR)
            {
                return bytesRead;
            }
            else
            {
                return ERROR;
            }
        }
        else if (retVal == 0)
        {
            // possible far end closing!
            return bytesRead;
        }

        bytesRead += retVal;
    }

    return bytesRead;
}


int radSocketWriteExact
(
    RADSOCK_ID      id,
    void            *buffer,
    int             lenToWrite
)
{
    UCHAR           *bPtr = (UCHAR *)buffer;
    int             retVal, bytesWritten = 0;

    while (bytesWritten < lenToWrite)
    {
        retVal = write (id->sockfd, bPtr+bytesWritten, lenToWrite-bytesWritten);

        if (retVal <= 0)
        {
            /*  ... write error
            */
            return retVal;
        }

        bytesWritten += retVal;
    }

    return bytesWritten;
}

int radSocketSetBlocking (RADSOCK_ID id, int isBlocking)
{
    int             flags;

    if ((flags = fcntl (id->sockfd, F_GETFL, 0)) < 0)
        return ERROR;
    
    if (!isBlocking)
        flags |= O_NONBLOCK;
    else
        flags &= ~O_NONBLOCK;
    
    if (fcntl (id->sockfd, F_SETFL, flags) < 0)
        return ERROR;
    
    return OK;
}
