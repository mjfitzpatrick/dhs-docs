#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "proto.h"

#define MAXNAME 128
#define LONG_SZ 20

/* AHA!  For the prototype, just open a file and write the data to it.
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
void
pipeSendPixelData (long *status, char *response, dhsHandle dhs_id,
                   void *data, size_t blkSize, struct fpConfig *pixelDescr,
                   double *exposure_id, char *obsSet_id)
{
   int fd;
   int write_cnt=0;
   char fname[MAXNAME];
   char exp_id[32];
   char cols[LONG_SZ];
   char rows[LONG_SZ];
   char column_idx[LONG_SZ];
   char row_idx[LONG_SZ];
   char datatype[LONG_SZ];
   char buf[45];
  
   /* something upstream failed.  exit or return? */
   if ((dhs_id != 0) || (*status < 0))
      return;
 
   /* convert exposure_id to ascii */  
   sprintf (exp_id, "%8.6f", *exposure_id); 
   strcpy (fname, obsSet_id);
   strcat (fname, ".");
   strcat (fname, exp_id); 

   fd = open (fname, O_CREAT|O_WRONLY|O_APPEND, S_IRUSR|S_IWUSR);
   if (fd == -1) {
      perror ("file open failed");
      *status = -1;
      strcpy (response, "pipeSendPixelData() fails to open file\n");
      return;
   }

   /* Write fpConfig struct to buf and then to file */
   bzero (buf, sizeof(buf));
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
   write_cnt = write (fd, buf, strlen(buf));
   if (write_cnt < 0) {
      perror ("write to file failed");
      *status = -1;
      strcpy (response, "pipeSendPixelData() fails to write file");
      close (fd);
      return;
   }
   write_cnt=0;

   /* Write raw pixel data to file */
   write_cnt = write (fd, data, blkSize);
   if (write_cnt < 0) {
      perror ("write to file failed");
      *status = -1;
      strcpy (response, "pipeSendPixelData() fails to write file");
      close(fd);
      return;
   }
   if (write_cnt == 0)
      printf ("2nd write of zero bytes\n");
   close (fd);
printf ("finished write of %d bytes in proto.c\n", write_cnt);

   *status = 1;
   strcpy (response, "pipeSendPixelData succeeds\n");
printf ("exiting proto.c\n");
   return;
}
