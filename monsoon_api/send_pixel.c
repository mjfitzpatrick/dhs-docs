#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "proto.h"

#define MAXNAME 128
#define LONG_SZ 20

void
dhsSendPixelData (long *status, char *response, dhsHandle dhs_id,
                   void *data, size_t blkSize, struct fpConfig *pixelDescr,
                   double *exposure_id, char *obsSet_id)
{
   int fd=0;
   int write_cnt=0;
   char fname[MAXNAME]="";
   char exp_id[32];
   char cols[LONG_SZ];
   char rows[LONG_SZ];
   char column_idx[LONG_SZ];
   char row_idx[LONG_SZ];
   char datatype[LONG_SZ];
   char buf[45];
   char *dir;
  
   /* something upstream failed.  exit or return? */
   if ((dhs_id != 0) || (*status < 0))
      return;
 
   /* get target directory */
   dir = getenv("MONSOON_DATA");
   if (dir == NULL) {
      dir = strdup ("/tmp/");  
   }
fprintf (stderr, "dir = %s\n", dir);

   if (chdir(dir) == -1) {
      fprintf (stderr, "Cannot chdir to %s\n", dir);
      free (dir);
      *status = -1;
      return;
   }
   free (dir);

   /* convert exposure_id to ascii */  
   strcpy (fname, obsSet_id);
   strcat (fname, ".");
   sprintf (exp_id, "%8.6f", *exposure_id); 
   strcat (fname, exp_id); 

   fd = open (fname, O_CREAT|O_WRONLY|O_APPEND, S_IRUSR|S_IWUSR);
   if (fd == -1) {
      perror ("file open failed");
      *status = -1;
      strcpy (response, "dhsSendPixelData() fails to open file\n");
      return;
   }

   /* other design would be to write pixelDescr to file directly without 
    * interpreting. */
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
      strcpy (response, "dhsSendPixelData() fails to write file");
      close (fd);
      return;
   }
   write_cnt=0;

   /* Write raw pixel data to file */
   write_cnt = write (fd, data, blkSize);
   if (write_cnt < 0) {
      perror ("write to file failed");
      *status = -1;
      strcpy (response, "dhsSendPixelData() fails to write file");
      close(fd);
      return;
   }
   if (write_cnt == 0)
      printf ("2nd write of zero bytes\n");
   close (fd);
printf ("finished write of %d bytes in proto.c\n", write_cnt);

   *status = 1;
   strcpy (response, "dhsSendPixelData succeeds\n");
printf ("exiting proto.c\n");
   return;
}
