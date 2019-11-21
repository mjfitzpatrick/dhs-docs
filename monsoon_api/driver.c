#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "proto.h"

/* prototype driver for newfirm dhs api.  This program will call all
 * functions defined in the API.
 */
void dhsSendPixelData (long *, char *, dhsHandle, void *, size_t,
                        struct fpConfig *, double *, char *);

void dhsSendMetaData (long *, char *, dhsHandle, void *, size_t,
                       struct mdConfig *, double *, char *);

int dhsSysOpen (long *, char *, int);

main()
{
   long status;
   char response[4096];
   if (dhsSysOpen (&status, response, 2)) {
      fprintf (stderr, "success of dhsSysOpen() call\n");
   } else {
      fprintf (stderr, "dhsSysOpen failed - %s\n", response);
   }
}
