#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "proto.h"

/* one call from Monsoon will contain 32 MB (2 arrays) of data */
#define BLOCKSZ 32000000

/* prototype driver for newfirm dhs api.  This program will call all
 * functions defined in the API.
 */
void pipeSendPixelData (long *, char *, dhsHandle, void *, size_t,
                        struct fpConfig *, double *, char *);

int
main(int argc, char **argv)
{
   long status;
   char response[4096];
   void *buf;
   size_t blkSize;
   char *obsSetID;
   double expID;
   dhsHandle dhs_id;
   struct fpConfig pan1;
   struct mdConfig meta;
   char *filename;
   FILE *fp;
   int fd, n=1;
 
   if (argc != 2) {
      fprintf (stderr, "USAGE: client data_filename\n");
      exit(-1);
   }

   filename = strdup ((const char *) argv[1]);

   fp = fopen (filename, "r");
   if (fp == NULL) {
      perror ("file read error");
      exit (-1);
   }
   fd = fileno(fp);

fprintf (stderr, "main() of driver - filename = %s\n", filename);

   expID = 77777777.111111;
   obsSetID = strdup ("Test_Observation_1");
   blkSize = BLOCKSZ;
   dhs_id = 0;
   status = 0;
   
   /* fill in mdConfig struct for exposure */
   meta.metaType = FITSHEADER;
   meta.numFields = 3;
   meta.fldSize[0] = 8;
   meta.fldSize[1] = 32;
   meta.fldSize[2] = 40;
   meta.dataType[0] = UBYTE;
   meta.dataType[1] = UBYTE; 
   meta.dataType[2] = UBYTE;
   
   /* fill in fpConfig struct for lower left array */
   pan1.xSize = 1000;
   pan1.ySize = 1000;
   pan1.xStart = 1;
   pan1.yStart = 1;
   pan1.dataType = CHAR;
   
fprintf (stderr, "start reading file in %ld size chunks\n", blkSize);
   /* open datafile from command line and read into the buffer until 
    * you reach the blkSize, then send the buffer, clear it and resume
    * reading where the fp left off.
    */
   buf = malloc (blkSize);
   if (buf == NULL) {
      perror ("malloc error");
      exit(-1);
   }
   while (n > 0)
   {
      bzero (buf, blkSize);

      if ((n = read (fd, buf, blkSize)) > 0) {
fprintf (stderr, "call to pipeSendPixelData()\n");
      pipeSendPixelData (&status, response, dhs_id, buf, n, &pan1,
		         &expID, obsSetID);
      } else if (n == -1) 
         perror ("file read error");
   }
   close (fd);

   free (obsSetID);
   free (filename);
   free (buf);
}
