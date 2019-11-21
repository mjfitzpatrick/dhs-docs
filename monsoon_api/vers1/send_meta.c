#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "proto.h"

#define MAXNAME 128
#define LONG_SZ 20

void
pipeSendMetaData (long *status, char *response, dhsHandle dhs_id,
                  void *data, size_t blkSize, struct mdConfig *metaDescr,
                  double *exposure_id, char *obsSet_id)
{
   int fd;
   int write_cnt=0;
   char fname[MAXNAME];
   char exp_id[32];
   char metaType[LONG_SZ];
   char numFields[LONG_SZ];
   char fldSize[DHS_MAXFIELDS][LONG_SZ];
   char datatype[DHS_MAXFIELDS][LONG_SZ];
   char buf[45];
   int i;

   /* something upstream failed. exit or return? */
   if ((dhs_id != 0) || (*status < 0))
      return;
 
   /* convert exposure_id to ascii */  
   sprintf (exp_id, "%6.5f", *exposure_id); 
   strcpy (fname, obsSet_id);
   strcat (fname, ".");
   strcat (fname, exp_id); 
   strcat (fname, "meta");

   fd = open (fname, O_CREAT|O_WRONLY|O_APPEND, S_IRUSR|S_IWUSR);
   if (fd == -1) {
      perror ("file open failed");
      *status = -1;
      strcpy (response, "pipeSendMetaData() fails to open file\n");
      return;
   }

   /* Write mdConfig struct to buf and then to file */
   bzero (buf, sizeof(buf));
   sprintf (metaType, "%u", metaDescr->metaType);
   strcpy (buf, metaType);
   strcat (buf, ":");
   sprintf (numFields, "%u", metaDescr->numFields);
   strcat (buf, numFields);
   strcat (buf, ":");
   for (i=0; i < metaDescr->numFields; i++)
   {
      sprintf (fldSize[i], "%u", metaDescr->fldSize[i]);
      strcat (buf, fldSize[i]);
      /* delimiter to separate array entries */
      strcat (buf, "|");
   }
   strcat (buf, ":");
   for (i=0; i < metaDescr->numFields; i++)
   {
      sprintf (datatype[i], "%u", metaDescr->dataType);
      strcat (buf, datatype[i]);
      /* delimiter to separate array entries */
      strcat (buf, "|");
   }
   strcat (buf, "\n");
   write_cnt = write (fd, buf, strlen(buf));
   if (write_cnt < 0) {
      perror ("write to file failed");
      *status = -1;
      strcpy (response, "pipeSendMetaData() fails to write file");
      close (fd);
      return;
   }
   write_cnt=0;

   /* Write raw metadata to file */
   write_cnt = write (fd, data, blkSize);
   if (write_cnt < 0) {
      perror ("write to file failed");
      *status = -1;
      strcpy (response, "pipeSendMetaData() fails to write file");
      close(fd);
      return;
   }
   if (write_cnt == 0)
      printf ("2nd write of zero bytes\n");
   close (fd);
printf ("finished write of %d bytes of metadata in send_meta.c\n", write_cnt);

   /* send success */
   *status = 1;
   strcpy (response, "pipeSendMetaData succeeds\n");
printf ("exiting send_meta.c\n");
   return;
}
