#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "proto.h"

typedef int dhsHandle; 

/* prototype driver for newfirm dhs api.  This program will call all
 * functions defined in the API.
 */
void pipeSendPixelData (long *, char *, dhsHandle, void *, size_t,
                        struct fpConfig *, double *, char *);

void pipeSendMetaData (long *, char *, dhsHandle, void *, size_t,
                       struct mdConfig *, double *, char *);

main()
{
   long status;
   char response[4096];
   char *buf;
   size_t blkSize;
   char *obsSetID;
   double expID;
   dhsHandle dhs_id;
   struct fpConfig pan1;
   struct mdConfig meta;
 
   expID = 77777777.111111;
   obsSetID = strdup ("Test_Observation_1");
   blkSize = 512;
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
   
   buf = new char[blkSize];
   bzero (buf, blkSize);
   strcpy (buf, "abcdefghi\n");

   pipeSendPixelData (&status, response, dhs_id, buf, blkSize, &pan1,
		      &expID, obsSetID);
   printf ("status = %ld response = %s", status, response);

   /* reset fpConfig struct for lower right array */
   pan1.xStart = 1001;
   expID += 1.01;

   pipeSendPixelData (&status, response, dhs_id, buf, blkSize, &pan1,
		      &expID, obsSetID);
   printf ("status = %ld response = %s", status, response);

   /* send metadata, but what to do about the identifier?  The MSD will
    * be the same as the pixel - name collision. SOLUTION: pipeSendMetaData
    * adds "meta" to the end of the filename. */

   bzero (buf, blkSize);
   strcpy (buf, "KEYWORD=VALUE\n");

   pipeSendMetaData (&status, response, dhs_id, buf, blkSize, &meta,
                     &expID, obsSetID);
   printf ("status = %ld response = %s", status, response);

   free (obsSetID);
}
