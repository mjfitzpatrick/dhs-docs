/* Prototype of sendPixelData() for Monsoon/DHS interface.  		*/
/* Author: Michelle Miller						*/
/* Date:   Thu Jul 29 16:20:31 MST 2004 (sockets add)      		*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include "proto.h"

#define MAX_BUF 1024
#define MAXNAME 128
#define LONG_SZ 20
#define HOSTNAME "pipen1.tuc.noao.edu"
#define PORT 9099
#define SOCKBUFSZ 96000

/* AHA!  For the prototype, just open a socket and write the data to it.
 * Assume everything is done with files.  The final output will be a FITS
 * file and @list to be used for running the science pipeline.  Then we
 * can take data from Monsoon and NOCS into this code and trigger the
 * science pipeline with it.  Simplifying assumption: files.  How will
 * I put the pieces of the array back together?
 */

/* What should I do with the focal plane info?  Merge the 4 files from 
 * each PAN's call to pipeSendPixelData() into one FITS file once I
 * have received all 4 pieces and the Metadata.  What should be used
 * as the staging area?  Just write the contents of fpConfig struct
 * at the top of the file in colon-separated ASCII fields.  What
 * combines them?
 */

/* alternate design: take in the fpConfig struct and use it in a call
 * to a DHS supervisor for use in putting mosaic back together.  Also,
 * pass the filename used to store the pixel data, so the supervisor
 * can find the data.  No need to store fpConfig struct in file as long
 * as you don't lose it.  better for fault tolerance to store it so the
 * system state can be recreated and no data is lost. Actually, a supervisor
 * could either farm out indiv. pieces to QuickLook and QuickReduce or 
 * wait for all 4 and send them as an entire focal plane.  Parameterize it.
 */
/* Hostname and port of server hard-coded for simplicity */
void
pipeSendPixelData (long *status, char *response, dhsHandle dhs_id,
                   void *data, size_t blkSize, struct fpConfig *pixelDescr,
                   double *exposure_id, char *obsSet_id)
{
   int write_cnt=0;
   char exp_id[32];
   char cols[LONG_SZ];
   char rows[LONG_SZ];
   char column_idx[LONG_SZ];
   char row_idx[LONG_SZ];
   char datatype[LONG_SZ];
   char buf[MAX_BUF];
  
   int pipe_socket, port; 
   long sockbufsize;
   struct sockaddr_in host_addr;
   struct hostent *hp;

   /* something upstream failed.  exit or return? */
   if ((dhs_id != 0) || (*status < 0))
      return;
 
fprintf (stderr, "entering pipeSendPixelData()\n");
   /* socket setup */
   pipe_socket = socket(AF_INET, SOCK_STREAM, 0);
   if (pipe_socket == -1) {
      perror("open socket error");
      exit(-1);
   }

   bzero((char *)&host_addr, sizeof(host_addr));
   hp = gethostbyname (HOSTNAME);
   if (hp == NULL) {
      perror("gethostbyname error");
      close(pipe_socket);
      exit(-1);
   }

   port = htons(PORT);
   bcopy(hp->h_addr_list[0],(char *)&host_addr.sin_addr.s_addr,hp->h_length);
   host_addr.sin_family = AF_INET;
   host_addr.sin_port = port;

fprintf (stderr, "socket = %d host_addr setup\n");
   /* adjust TCP window size to optimal size found experimentally */
   sockbufsize = SOCKBUFSZ;
   if (setsockopt (pipe_socket, SOL_SOCKET, SO_SNDBUF, &sockbufsize, 
                   sizeof(long)) < 0) {
      perror("setsockopt error");
      close(pipe_socket);
      exit(-1);
   }
 
   if (connect (pipe_socket, (struct sockaddr *)&host_addr, sizeof(host_addr)) == -1) {
      perror("connect failed");
      close(pipe_socket);
      exit(-1);
   }

fprintf (stderr, "connect to server succeeded\n");
#if 0
   /* Convert fpConfig struct to ascii colon-delimited fields, write to
      buf, and send */
   bzero (buf, sizeof(buf));
   strcpy (buf, obsSet_id);
   strcat (buf, ":");
   sprintf (exp_id, "%8.6f", *exposure_id); 
   strcat (buf, exp_id); 
   strcat (buf, ":");
   sprintf (cols, "%u", pixelDescr->xSize);
   strcpy (buf, cols);
   strcat (buf, ":");
   sprintf (rows, "%u", pixelDescr->ySize);
   strcat (buf, rows);
   strcat (buf, ":");
   sprintf (column_idx, "%u", pixelDescr->xStart);
   strcat (buf, column_idx);
   strcat (buf, ":");
   sprintf (row_idx, "%u", pixelDescr->yStart);
   strcat (buf, row_idx);
   strcat (buf, ":");
   sprintf (datatype, "%u", pixelDescr->dataType);
   strcat (buf, datatype);
   strcat (buf, "\n");
   write_cnt = write (pipe_socket, buf, strlen(buf));
fprintf (stderr, "send buffer size = %d, wrote %d bytes\n",
         strlen(buf), write_cnt);
   if (write_cnt < 0) {
      perror ("write to file failed");
      *status = -1;
      strcpy (response, "pipeSendPixelData() fails to write file");
      close (pipe_socket);
      return;
   }
   write_cnt=0;
#endif

   /* Write raw pixel data to socket */
   write_cnt = write (pipe_socket, data, blkSize);
fprintf (stderr, "send buffer size = %d, wrote %d bytes\n", blkSize, write_cnt);
   if (write_cnt < 0) {
      perror ("write to file failed");
      *status = -1;
      strcpy (response, "pipeSendPixelData() fails to write file");
      close(pipe_socket);
      return;
   }
   if (write_cnt == 0)
      printf ("2nd write of zero bytes\n");
   close (pipe_socket);
printf ("finished write of %d bytes in proto.c\n", write_cnt);

   *status = 1;
   strcpy (response, "pipeSendPixelData succeeds\n");
printf ("exiting proto.c\n");
   return;
}
