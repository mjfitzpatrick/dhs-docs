/* Implementation of Monsoon/DHS API for NEWFIRM - vers. 5              */
/* Author: Michelle Miller                                              */
/* Date:   Sat May 21 16:30:50 MST 2005                                 */

#include "proto.h"

/* dhsOpenConnect()                                                     *
 *                                                                      *
 * Create persistent connection with node here.  Need obs set id   *
 * and number of exposures expected to help supervisor schedule         *
 * resource and track status of the sequence.  dhsHandle can then be    *
 * the socket number.  No need to keep DHS machine name here if		*
 * supervisor can identify me. Make more sense for NOCS to send         *
 * mdConfig instead of fpConfig. */

/*int 
dhsOpenConnect (long *status, char *response, dhsHandle *dhsID, ulong whoAmI,
                struct fpConfig *pxlConfig) */
/* proposed interface */
int 
dhsOpenConnect (long *status, char *response, dhsHandle *dhsID,
                unsigned long whoAmI, struct fpConfig *pxlConfig)
{
   char *supervisor;
   int port, supervisor_sock;
   char buf[LINE_SZ];
   char caller[LONG_SZ];
   int sockbufsize=LINE_SZ;
   char *ascii_port;

   if (status == ERROR)
      return(-1);

   /* find supervisor node */
   supervisor = getenv("DHS_SUPERVISOR");
   if (supervisor == NULL) {
      supervisor = strdup ("localhost");
   }
fprintf (stderr, "DHS_SUPERVISOR = %s\n", node);
                                                                                
   ascii_port = getenv("DHS_PORT");
   if (ascii_port == NULL) {
      port = 9099;  /* default */
   } else {
      port = atoi (ascii_port);
   }
                                                                                
   /* socket setup and connect */
/* utility function at wrong level - I can't try to reconnect.  I would have to redeclare a socket. */
   supervisor_sock = connectTo (supervisor, port);
   if (supervisor_sock == -1) {
      *status = ERROR;
      response = strdup ("dhsOpenConnect() cannot connect to Supervisor");
      return (ERROR);
   }

   /* send message to supervisor asking which node to send its data to */
   /* msg = "OPENCONNECT:whoAmI" */
   bzero(buf, sizeof(buf));
   sprintf (buf, "%d", OPENCONNECT);
   strcat (buf, ":");
   sprintf (caller, "%u", whoAmI)
   strcat (buf, caller);
   write (supervisor_sock, buf, strlen(buf));

 /* if this is NOCS, shouldn't it send metadata descriptor? */
   write (supervisor_sock, pxlConfig, sizeof(pxlConfig));

/* receive address of chosen node and tear down connection */
/* open connection to node and store socket id in dhsHandle */
/* update status and return */
}

/* dhsOpenExp()                                                         *
 *                                                                      *
 * Function to notify a DHS node that an exposure is beginning.  Focal  *
 * plane information is passed as well as the exposure unique           *
 * identifier and identifier for the observation set. unique names for  *
 * arrays? could I use x and y coordinates (see fpConfig)? */
int 
dhsOpenExp (long *status, char *response, dhsHandle dhsID, 
            struct fpConfig *pxlConfig, double *expID, char *obsSetID)
/* revised */
int 
dhsOpenExp (long *status, char *response, dhsHandle *dhsID, int channel,
            struct fpConfig *pxlConfig, double *expID, char *obsSetID)
{
/* socket setup */
/* connect to dhsID node given in dhsOpenConnect() */
/* send message to node to expect an exposure called blah - need this? */
/* receive ACK, update status and return socket_id for data send */
}

/* later revision */
int 
dhsOpenExp (long *status, char *response, dhsHandle *dhsID,
            struct fpConfig *pxlConfig, double *expID, char *obsSetID)
{
/* send message to node using dhsHandle to expect exposure x of y */
/* receive ACK, update status and return */
}

/* dhsCloseExp()                                                        *
 *                                                                      *
 * Function to notify DHS that all data related to the exposure         *
 * specified have been sent and that any current exposures that are     *
 * not yet complete will not be completed.  The DHS can cleanup from    *
 * this exposure and prepare for the next exposure.                     */
/* MM - timing problem? what if PAN sends dhsCloseExp() before a        *
 * dhsSendPixelData() has finished? */
int 
dhsCloseExp (long *status, char *response, dhsHandle dhsID, double expID)
{
/* send message to notify Collector of end of exposure */
/* receive ACK, update status and return */
}

/* dhsCloseConnect()                                                    *
 *                                                                      *
 * Function to close a connection to a DHS node.  This function is      *
 * called when no additional exposures or observations will be          *
 * initiated.  Any observations which are not complete will not be      *
 * completed.  The DHS node can cleanup and shutdown.                   */
int 
dhsCloseConnect (long *status, char *response, dhsHandle dhsID)
{
/* send message to notify Collector of the end of data stream */
/* receive ACK and tear down connection */
/* update status and return */
}

/* dhsSysClose()                                                        *
 *                                                                      *
 * Function to notify DHS Supervisor that NOCS and Monsoon are shutting *
 * down and that the DHS Supervisor can shutdown.  All observations are *
 * complete.                                                            */
int 
dhsSysClose (long *status, char *response, dhsHandle dhsID)
{
/* setup socket connection to supervisor */
/* send message to stop collectors and shutdown (unless daemon) */
/* receive ACK and tear down connection */
/* update status and return */
}

/* dhsSendPixelData()                                                   *
 *                                                                      *
 * Function to send pixel data to a DHS node for an exposure. Data is   *
 * formatted starting in the lower left corner and  advancing in        *
 * row-major order.                                                     */
int
dhsSendPixelData (long *status, char *response, dhsHandle dhsID, 
                  void *pxlAddr, size_t blkSize, struct fpConfig *pxlDescr,
                  double *expID, char *obsSetID)
{
/* use existing persistent connection */
}

/* dhsSendMetaData()                                                     *
 *                                                                       *
 * Function to send metadata to a DHS node for an exposure.              */
int
dhsSendMetaData (long *status, char *response, dhsHandle dhsID,
                 void *blkAddr, size_t blkSize, struct mdConfig *mdDescr,
                 double *expID, char *obsSetID)
{
}

/* dhsIOCtl()                                                            *
 *                                                                       *
 * Function to change the underlying DHS behavior. (mm- needed?) */
int
dhsIOCtl (long *status, char *response, dhsHandle dhsID, ulong ioCtlFunc,
          char *ObsID, double *expID,...)
{
}
