struct fpConfig {
   unsigned xSize;		/* Number of Columns - size of a row       */
   unsigned ySize;		/* Number of Rows - size of a column       */
   unsigned xStart;		/* First pixel's column index              */
   unsigned yStart;     	/* First pixel's row index                 */
   long dataType;       	/* Pixel data type - number of bytes/pixel */
};

enum dataType {UBYTE, BYTE, USHORT, INTEGER, UNSIGNED, VLONG, UVLONG,
               FLOAT, DOUBLE, CHAR};

typedef int dhsHandle; 

/* where is MAXFIELDS defined? Don't include directly as it includes too
 * much other stuff.
#include "dhsUtil.h"
 */

#define DHS_MAXFIELDS 4096

struct mdConfig {
   unsigned metaType;		/* Metadata classification */
   unsigned numFields; 		/* Number of fields in each metadata entry */
   unsigned fldSize[DHS_MAXFIELDS]; /* For each field, how many elements of a datatype */
   unsigned dataType[DHS_MAXFIELDS]; /* Datatype of each field */
};

enum metaType {FITSHEADER, AVPAIR, SHIFTLIST, ARRAYDATA};

