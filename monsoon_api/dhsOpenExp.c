/* Implementation of Monsoon/DHS API for NEWFIRM - vers. 5              */
/* Author: Michelle Miller                                              */
/* Date:   Sat May 21 16:30:50 MST 2005                                 */

#include "proto.h"

/* dhsOpenConnect()                                                     *
 *                                                                      *
 * Function to configure a DHS node and connect to it.  Configuration,  *
 * pixel data and metadata are sent to DHS node via this connection.    */

/* MM - create persistent connection with node here.  Need obs set id   *
 * and number of exposures expected to help supervisor schedule         *
 * resource and track status of the sequence.  dhsHandle can then be    *
 * the socket number.  No need to keep DHS machine name here if		*
 * supervisor can identify me. */

int 
dhsOpenConnect (long *status, char *response, dhsHandle *dhsID, ulong whoAmI,
                struct fpConfig *pxlConfig)
{
/* how does calling function know location of supervisor? */
/* socket setup */
/* connect */
/* send message to supervisor asking which node to send its data to */
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
