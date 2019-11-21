/*******************************************************************************
 * include(s):
 *******************************************************************************/
#if !defined(_dhsTcl_H_)
 #include "dhsTcl.h"
#endif

/*******************************************************************************
 * global(s):
 *******************************************************************************/
#define DHS_TCL_FALSE               0
#define DHS_TCL_TRUE (!DHS_TCL_FALSE)
#define DHS_TCL_NAME_LEN           20
#define DHS_TCL_RESULT_LEN         64
#define DHS_TCL_ITEM_LEN           64
#define DHS_TCL_EXAMPLE_LEN       100
#define DHS_TCL_HELP_LEN         8192

static long dhsLstat = 0L;
static char dhsResponse[MAXMSG] = "";
static char dhsResult[DHS_TCL_RESULT_LEN] = "";
static char dhsHelp[DHS_TCL_HELP_LEN] = "";
static int dhsRoom = DHS_TCL_TRUE;
static int dhsNchars = 0;

/*******************************************************************************
 * structure(s)
 *******************************************************************************/
typedef struct help_str {
  char name[DHS_TCL_NAME_LEN];
  char help[DHS_TCL_ITEM_LEN];
  char example[DHS_TCL_EXAMPLE_LEN];
} help_t, *help_p, **help_s;

static help_t usage[] =  {
  {"DHS               ","MONSOON Data Handling System",_dhsTcl_S_},
  {"                  ","                            ",""},
  {"Command           ","Description                 ","Example"},
  {"-------           ","-----------                 ","-------"},
  {"dhs::SysOpen      ","Open the DHS system         ","set dhsID [dhs::SysOpen <systemID>]"},
  {"dhs::SysClose     ","Close the DHS system        ","set dhsStatus [dhs::SysClose <dhsID>]"},
  {"dhs::OpenConnect  ","Open the DHS connection     ","set dhsID [dhs::OpenConnect <systemID> <{configList}>]"},
  {"dhs::CloseConnect ","Close the DHS connection    ","set dhsStatus [dhs::CloseConnect <dhsID>]"},
  {"dhs::OpenExp      ","Open the DHS exposure       ","set dhsStatus [dhs::OpenExp <dhsID> <{configList}> <expID> <obsetID>]"},
  {"dhs::CloseExp     ","Close the DHS exposure      ","set dhsStatus [dhs::CloseExp <dhsID> <expID>]"},
  {"dhs::IOctl        ","Perform some IOCTL function ","set dhsStatus [dhs::IOctl <dhsID> <ioctl> <expID> <obsetID>]"},
  {"dhs::ReadImage    ","Read an image               ","set dhsStatus [dhs::ReadImage <dhsID>]"},
  {"dhs::PixelData    ","Send an image               ","set dhsStatus [dhs::PixelData <dhsID> <{data}> <nelms> <{configList}> <expID> <obsetID>]"},
  {"dhs::MetaData     ","Send meta-data block        ","set dhsStatus [dhs::MetaData <dhsID> <{data}> <nelms> <{configList}> <expID> <obsetID>]"}
};
#define NCMDS (sizeof(usage)/sizeof(help_t))

/*******************************************************************************
 * dhsHelpTcl ( ... )
 *******************************************************************************/
static int dhsHelpTcl ( ClientData clientData, Tcl_Interp *interp, int argc, char *argv[] ) {
  /* declare local scope variables */
  int dhsIc = 0;
  /* reset static variable(s) */
  dhsNchars = 0;
  dhsRoom = DHS_TCL_TRUE;
  (void) memset(dhsHelp,'\0',DHS_TCL_HELP_LEN);
  /* write out everything else */
  for ( dhsIc=0; dhsIc<NCMDS; dhsIc++ ) {
   if ( ! (dhsRoom=( (DHS_TCL_HELP_LEN-dhsNchars) > (DHS_TCL_NAME_LEN+(DHS_TCL_ITEM_LEN*2)) ? DHS_TCL_TRUE : DHS_TCL_FALSE )) ) break;
   dhsNchars += sprintf((char *)&dhsHelp[dhsNchars],"%s %s %s\n",usage[dhsIc].name,usage[dhsIc].help,usage[dhsIc].example);
  }
  /* set result and return */
  (void) Tcl_SetResult(interp,dhsHelp,TCL_STATIC);
  return TCL_OK;
}

/*******************************************************************************
 * dhsReadImageTcl ( ... )
 *  Use: set dhsStatus [dhs::ReadImage <dhsID>]
 *******************************************************************************/
static int dhsReadImageTcl ( ClientData clientData, Tcl_Interp *interp, int argc, char *argv[] ) {
  /* initialize variables */
  dhsHandle dhsID = (dhsHandle)0;
  /* check handle */
  if ( Tcl_GetInt(interp,argv[1],(int *)&dhsID) != TCL_OK ) {
    (void) sprintf(dhsResult,"%s","dhsReadImageTcl-E-bad handle\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* execute the dhs function */
  dhsLstat = 0L;
  (void) memset(dhsResponse,'\0',MAXMSG);
  dhsReadImage(&dhsLstat,dhsResponse,dhsID);
  if ( STATUS_BAD(dhsLstat) ) {
    (void) sprintf(dhsResult,"%s",dhsResponse);
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* return result */  
  (void) sprintf(dhsResult,"%ld",dhsLstat);
  (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
  return TCL_OK;
}

/*******************************************************************************
 * dhsIOctlTcl ( ... )
 *  Use: set dhsStatus [dhs::IOctl <dhsID> <ioctl> <expID> <obsetID>]
 *******************************************************************************/
static int dhsIOctlTcl ( ClientData clientData, Tcl_Interp *interp, int argc, char *argv[] ) {
  /* initialize variables */
  char obsetID[MAXMSG] = {'\0'};
  dhsHandle dhsID = (dhsHandle)0;
  double expID = (double)0.0;
  int inum = 0;
  int ic = 0;
  /* check handle */
  if ( Tcl_GetInt(interp,argv[1],(int *)&dhsID) != TCL_OK ) {
    (void) sprintf(dhsResult,"%s","dhsIOctlTcl-E-bad handle\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* check ioctl */
  if ( Tcl_GetInt(interp,argv[2],(int *)&inum) != TCL_OK ) {
    (void) sprintf(dhsResult,"%s","dhsIOctlTcl-E-bad ioctl\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* check expID */
  if ( Tcl_GetDouble(interp,argv[3],&expID) != TCL_OK ) {
    (void) sprintf(dhsResult,"%s","dhsIOctlTcl-E-bad exposure id\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* check obsetID */
  (void) memset((void *)obsetID,'\0',MAXMSG);
  for ( ic=4; ic<argc; ic++ ) { strcat(obsetID,argv[ic]); strcat(obsetID," "); }
  /* execute the dhs function */
  dhsLstat = 0L;
  (void) memset(dhsResponse,'\0',MAXMSG);
  dhsIOCtl(&dhsLstat,dhsResponse,dhsID,(ulong)inum,&expID,obsetID,NULL);
  if ( STATUS_BAD(dhsLstat) ) {
    (void) sprintf(dhsResult,"%s",dhsResponse);
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* return result */  
  (void) sprintf(dhsResult,"%ld",dhsLstat);
  (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
  return TCL_OK;
}

/*******************************************************************************
 * dhsMetaDataTcl ( ... )
 *  Use: set dhsStatus [dhs::MetaData <dhsID> <{data}> <nlines> <{configList}> <expID> <obsetID>]
 *******************************************************************************/
static int dhsMetaDataTcl ( ClientData clientData, Tcl_Interp *interp, int argc, char *argv[] ) {
  /* initialize variables */
  char *fp = (char *)NULL;
  char *ap = (char *)NULL;
  char *cp = (char *)NULL;
  char obsetID[MAXMSG] = {'\0'};
  dhsHandle dhsID = (dhsHandle)0;
  double expID = (double)0.0;
  mdConfig_t mdConfigTcl = {0L};
  int ierror = 0;
  int nbytes = 0;
  int nlines = 0;
  int ic = 0;
  int ik = 0;
  int lsArgcC = 0;
  char **lsArgvC = (char **)NULL;
  int lsArgcD = 0;
  char **lsArgvD = (char **)NULL;
  int lsArgcF = 0;
  char **lsArgvF = (char **)NULL;
  int lsArgcS = 0;
  char **lsArgvS = (char **)NULL;
  char fitsName[DHS_FITS_NAMESIZE] = {'\0'};
  char fitsValue[DHS_FITS_VALSIZE] = {'\0'};
  char fitsComment[DHS_FITS_COMMENT] = {'\0'};
  char avpName[DHS_AVP_NAMESIZE] = {'\0'};
  char avpValue[DHS_AVP_VALSIZE] = {'\0'};
  char avpComment[DHS_AVP_COMMENT] = {'\0'};
  /* check handle */
  if ( Tcl_GetInt(interp,argv[1],(int *)&dhsID) != TCL_OK ) {
    (void) sprintf(dhsResult,"%s","dhsMetaDataTcl-E-bad handle\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* check data list */
  if ( Tcl_SplitList(interp,argv[2],&lsArgcD,&lsArgvD)!=TCL_OK ) {
    (void) sprintf(dhsResult,"%s","dhsMetaDataTcl-E-bad data list\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* check nlines */
  if ( Tcl_GetInt(interp,argv[3],&nlines) != TCL_OK ) {
    (void) sprintf(dhsResult,"%s","dhsMetaDataTcl-E-bad nlines\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* check configuration list */
  if ( Tcl_SplitList(interp,argv[4],&lsArgcC,&lsArgvC)!=TCL_OK || lsArgcC!=4L ) {
    (void) sprintf(dhsResult,"%s","dhsMetaDataTcl-E-bad configuration list\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* check expID */
  if ( Tcl_GetDouble(interp,argv[5],&expID) != TCL_OK ) {
    (void) sprintf(dhsResult,"%s","dhsMetaDataTcl-E-bad exposure id\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* check obsetID */
  (void) memset((void *)obsetID,'\0',MAXMSG);
  for ( ic=6; ic<argc; ic++ ) { strcat(obsetID,argv[ic]); strcat(obsetID," "); }
  obsetID[strlen(obsetID)-1] = '\0';
  /* set configuration */
  (void) memset((void *)&mdConfigTcl,0,sizeof(mdConfig_t));
  if ( Tcl_GetInt(interp,lsArgvC[0],(int *)&(mdConfigTcl.metaType))  != TCL_OK ) ierror = DHS_TCL_TRUE;
  if ( Tcl_GetInt(interp,lsArgvC[1],(int *)&(mdConfigTcl.numFields)) != TCL_OK ) ierror = DHS_TCL_TRUE;
  if ( Tcl_SplitList(interp,lsArgvC[2],&lsArgcF,&lsArgvF)            != TCL_OK ) ierror = DHS_TCL_TRUE;
  if ( Tcl_SplitList(interp,lsArgvC[3],&lsArgcS,&lsArgvS)            != TCL_OK ) ierror = DHS_TCL_TRUE;
  if ( ierror ) {
    (void) sprintf(dhsResult,"%s","dhsMetaDataTcl-E-bad list element\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  for ( ic=0; ic<mdConfigTcl.numFields; ic++ ) {
    if ( Tcl_GetInt(interp,lsArgvF[ic],(int *)&(mdConfigTcl.fieldSize[ic])) != TCL_OK ) ierror = DHS_TCL_TRUE;
    if ( Tcl_GetInt(interp,lsArgvS[ic],(int *)&(mdConfigTcl.dataType[ic]))  != TCL_OK ) ierror = DHS_TCL_TRUE;
    if ( ierror ) {
      (void) sprintf(dhsResult,"%s","dhsMetaDataTcl-E-bad configuration element\n");
      (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
      return TCL_ERROR;
    }
  }
  /* get memory */
  switch (mdConfigTcl.metaType) {
    case DHS_MDTYPE_FITSHEADER:
      nbytes = nlines*DHS_FITS_RAWLEN;
      if ( (cp=fp=(char *)Tcl_Alloc(nbytes)) == (char *)NULL ) {
        (void) sprintf(dhsResult,"%s","dhsMetaDataTcl-E-bad alloc\n");
        (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
        return TCL_ERROR;
      }
      (void) memset((void *)fp,' ',nbytes);
      for ( ic=0, ik=0; ik<nlines*3; ik+=3 ) {
        (void) sprintf((char *)fitsName,"%8s",lsArgvD[ik]);
        (void) memmove((void *)fp,fitsName,DHS_FITS_NAMESIZE);   fp += DHS_FITS_NAMESIZE;
        (void) sprintf((char *)fitsValue,"%20s",lsArgvD[ik+1]);
        (void) memmove((void *)fp,fitsValue,DHS_FITS_VALSIZE);   fp += DHS_FITS_VALSIZE;
        (void) sprintf((char *)fitsComment,"%46s",lsArgvD[ik+2]);
        (void) memmove((void *)fp,fitsComment,DHS_FITS_COMMENT); fp += DHS_FITS_COMMENT;
      }
      break;
    case DHS_MDTYPE_AVPAIR:
      nbytes = nlines*DHS_AVP_RAWLEN;
      if ( (cp=ap=(char *)Tcl_Alloc(nbytes)) == (char *)NULL ) {
        (void) sprintf(dhsResult,"%s","dhsMetaDataTcl-E-bad alloc\n");
        (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
        return TCL_ERROR;
      }
      (void) memset((void *)ap,' ',nbytes);
      for ( ic=0, ik=0; ik<nlines*3; ik+=3 ) {
        (void) sprintf((char *)avpName,"%32s",lsArgvD[ik]);
        (void) memmove((void *)ap,avpName,DHS_AVP_NAMESIZE);   ap += DHS_AVP_NAMESIZE;
        (void) sprintf((char *)avpValue,"%32s",lsArgvD[ik+1]);
        (void) memmove((void *)ap,avpValue,DHS_AVP_VALSIZE);   ap += DHS_AVP_VALSIZE;
        (void) sprintf((char *)avpComment,"%64s",lsArgvD[ik+2]);
        (void) memmove((void *)ap,avpComment,DHS_AVP_COMMENT); ap += DHS_AVP_COMMENT;
      }
      break;
    default:
      (void) sprintf(dhsResult,"%s","dhsMetaDataTcl-E-bad data type\n");
      (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
      return TCL_ERROR;
      break;
  }
  /* execute the dhs function */
  dhsLstat = 0L;
  (void) memset(dhsResponse,'\0',MAXMSG);
  dhsSendMetaData(&dhsLstat,dhsResponse,dhsID,(void *)cp,(size_t)nbytes,&mdConfigTcl,&expID,obsetID);
  if ( STATUS_BAD(dhsLstat) ) {
    (void) sprintf(dhsResult,"%s",dhsResponse);
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* return result */
  (void) sprintf(dhsResult,"%ld",dhsLstat);
  (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
  (void) Tcl_Free((char *)lsArgvC);
  (void) Tcl_Free((char *)lsArgvD);
  (void) Tcl_Free((char *)lsArgvF);
  (void) Tcl_Free((char *)lsArgvS);
  (void) Tcl_Free((char *)cp);
  return TCL_OK;
}

/*******************************************************************************
 * dhsPixelDataTcl ( ... )
 *  Use: set dhsStatus [dhs::PixelData <dhsID> <{data}> <nelms> <{configList}> <expID> <obsetID>]
 *******************************************************************************/
static int dhsPixelDataTcl ( ClientData clientData, Tcl_Interp *interp, int argc, char *argv[] ) {
  /* initialize variables */
  ulong *ip = (ulong *)NULL;
  ulong *dp = (ulong *)NULL;
  char obsetID[MAXMSG] = {'\0'};
  dhsHandle dhsID = (dhsHandle)0;
  double expID = (double)0.0;
  fpConfig_t fpConfigTcl = {0L};
  int ierror = 0;
  int nbytes = 0;
  int nelms = 0;
  int ival = 0;
  int ic = 0;
  int ik = 0;
  int lsArgcC = 0;
  char **lsArgvC = (char **)NULL;
  int lsArgcD = 0;
  char **lsArgvD = (char **)NULL;
  /* check handle */
  if ( Tcl_GetInt(interp,argv[1],(int *)&dhsID) != TCL_OK ) {
    (void) sprintf(dhsResult,"%s","dhsPixelDataTcl-E-bad handle\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* check data list */
  if ( Tcl_SplitList(interp,argv[2],&lsArgcD,&lsArgvD)!=TCL_OK ) {
    (void) sprintf(dhsResult,"%s","dhsPixelDataTcl-E-bad data list\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* check nelms */
  if ( Tcl_GetInt(interp,argv[3],&nelms) != TCL_OK ) {
    (void) sprintf(dhsResult,"%s","dhsPixelDataTcl-E-bad nelms\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* check configuration list */
  if ( Tcl_SplitList(interp,argv[4],&lsArgcC,&lsArgvC)!=TCL_OK || lsArgcC!=5L ) {
    (void) sprintf(dhsResult,"%s","dhsPixelDataTcl-E-bad configuration list\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* check expID */
  if ( Tcl_GetDouble(interp,argv[5],&expID) != TCL_OK ) {
    (void) sprintf(dhsResult,"%s","dhsPixelDataTcl-E-bad exposure id\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* check obsetID */
  (void) memset((void *)obsetID,'\0',MAXMSG);
  for ( ic=6; ic<argc; ic++ ) { strcat(obsetID,argv[ic]); strcat(obsetID," "); }
  obsetID[strlen(obsetID)-1] = '\0';
  /* set configuration */
  (void) memset((void *)&fpConfigTcl,0,sizeof(fpConfig_t));
  if ( Tcl_GetInt(interp,lsArgvC[0],(int *)&(fpConfigTcl.xSize))    != TCL_OK ) ierror = DHS_TCL_TRUE;
  if ( Tcl_GetInt(interp,lsArgvC[1],(int *)&(fpConfigTcl.ySize))    != TCL_OK ) ierror = DHS_TCL_TRUE;
  if ( Tcl_GetInt(interp,lsArgvC[2],(int *)&(fpConfigTcl.xStart))   != TCL_OK ) ierror = DHS_TCL_TRUE;
  if ( Tcl_GetInt(interp,lsArgvC[3],(int *)&(fpConfigTcl.yStart))   != TCL_OK ) ierror = DHS_TCL_TRUE;
  if ( Tcl_GetInt(interp,lsArgvC[4],(int *)&(fpConfigTcl.dataType)) != TCL_OK ) ierror = DHS_TCL_TRUE;
  if ( ierror ) {
    (void) sprintf(dhsResult,"%s","dhsPixelDataTcl-E-bad list element\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* get memory */
  nbytes = nelms * sizeof(ulong);
  if ( (ip=dp=(ulong *)Tcl_Alloc(nbytes)) == (ulong *)NULL ) {
    (void) sprintf(dhsResult,"%s","dhsPixelDataTcl-E-bad alloc\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* extract data by re-using lsArgcC/lsArgvC */
  (void) memset((void *)dp,0,nbytes);
  for ( ic=0; ic<lsArgcD; ic++ ) {
    if ( Tcl_SplitList(interp,lsArgvD[ic],&lsArgcC,&lsArgvC) != TCL_OK ) {
      (void) sprintf(dhsResult,"%s","dhsPixelDataTcl-E-bad internal list\n");
      (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
      return TCL_ERROR;
    }
    for ( ik=0; ik<lsArgcC; ik++ ) {
      ival = 0;
      if ( Tcl_GetInt(interp,lsArgvC[ik],&ival) != TCL_OK ) {
        (void) sprintf(dhsResult,"%s","dhsPixelDataTcl-E-bad array data\n");
        (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
        return TCL_ERROR;
      }
      *dp = (ulong)ival;
      dp++;
    }
  }
  /* execute the dhs function */
  dhsLstat = 0L;
  (void) memset(dhsResponse,'\0',MAXMSG);
  dhsSendPixelData(&dhsLstat,dhsResponse,dhsID,(void *)ip,(size_t)nbytes,&fpConfigTcl,&expID,obsetID);
  if ( STATUS_BAD(dhsLstat) ) {
    (void) sprintf(dhsResult,"%s",dhsResponse);
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* return result */
  (void) sprintf(dhsResult,"%ld",dhsLstat);
  (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
  (void) Tcl_Free((char *)lsArgvC);
  (void) Tcl_Free((char *)lsArgvD);
  (void) Tcl_Free((char *)ip);
  return TCL_OK;
}

/*******************************************************************************
 * dhsOpenExpTcl ( ... )
 *  Use: set dhsStatus [dhs::OpenExp <dhsID> <{configList}> <expID> <obsetID>]
 *******************************************************************************/
static int dhsOpenExpTcl ( ClientData clientData, Tcl_Interp *interp, int argc, char *argv[] ) {
  /* initialize variables */
  char obsetID[MAXMSG] = {'\0'};
  dhsHandle dhsID = (dhsHandle)0;
  double expID = (double)0.0;
  fpConfig_t fpConfigTcl = {0L};
  int ierror = DHS_TCL_FALSE;
  int ic = 0;
  int lsArgc = 0;
  char **lsArgv = (char **)NULL;
  /* check handle */
  if ( Tcl_GetInt(interp,argv[1],(int *)&dhsID) != TCL_OK ) {
    (void) sprintf(dhsResult,"%s","dhsOpenExpTcl-E-bad handle\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* check configuration list */
  if ( Tcl_SplitList(interp,argv[2],&lsArgc,&lsArgv)!=TCL_OK || lsArgc<5L ) {
    (void) sprintf(dhsResult,"%s","dhsOpenExpTcl-E-bad list\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* check expID */
  if ( Tcl_GetDouble(interp,argv[3],&expID) != TCL_OK ) {
    (void) sprintf(dhsResult,"%s","dhsOpenExpTcl-E-bad exposure id\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* check obsetID */
  (void) memset((void *)obsetID,'\0',MAXMSG);
  for ( ic=4; ic<argc; ic++ ) { strcat(obsetID,argv[ic]); strcat(obsetID," "); }
  /* set configuration */
  (void) memset((void *)&fpConfigTcl,0,sizeof(fpConfig_t));
  if ( Tcl_GetInt(interp,lsArgv[0],(int *)&(fpConfigTcl.xSize))    != TCL_OK ) ierror = DHS_TCL_TRUE;
  if ( Tcl_GetInt(interp,lsArgv[1],(int *)&(fpConfigTcl.ySize))    != TCL_OK ) ierror = DHS_TCL_TRUE;
  if ( Tcl_GetInt(interp,lsArgv[2],(int *)&(fpConfigTcl.xStart))   != TCL_OK ) ierror = DHS_TCL_TRUE;
  if ( Tcl_GetInt(interp,lsArgv[3],(int *)&(fpConfigTcl.yStart))   != TCL_OK ) ierror = DHS_TCL_TRUE;
  if ( Tcl_GetInt(interp,lsArgv[4],(int *)&(fpConfigTcl.dataType)) != TCL_OK ) ierror = DHS_TCL_TRUE;
  if ( ierror ) {
    (void) sprintf(dhsResult,"%s","dhsOpenExpTcl-E-bad list element\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* execute the dhs function */
  dhsLstat = 0L;
  (void) memset(dhsResponse,'\0',MAXMSG);
  dhsOpenExp(&dhsLstat,dhsResponse,dhsID,&fpConfigTcl,&expID,obsetID);
  if ( STATUS_BAD(dhsLstat) ) {
    (void) sprintf(dhsResult,"%s",dhsResponse);
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* return result */
  (void) sprintf(dhsResult,"%ld",dhsLstat);
  (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
  (void) Tcl_Free((char *)lsArgv);
  return TCL_OK;
}

/*******************************************************************************
 * dhsCloseExpTcl ( ... )
 *  Use: set dhsStatus [dhs::CloseExp <dhsID> <expID>]
 *******************************************************************************/
static int dhsCloseExpTcl ( ClientData clientData, Tcl_Interp *interp, int argc, char *argv[] ) {
  /* initialize variables */
  dhsHandle dhsID = (dhsHandle)0;
  double expID = (double)0.0;
  /* check handle */
  if ( Tcl_GetInt(interp,argv[1],(int *)&dhsID) != TCL_OK ) {
    (void) sprintf(dhsResult,"%s","dhsCloseExpTcl-E-bad handle\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* check expID */
  if ( Tcl_GetDouble(interp,argv[2],&expID) != TCL_OK ) {
    (void) sprintf(dhsResult,"%s","dhsCloseExpTcl-E-bad exposure id\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* execute the dhs function */
  dhsLstat = 0L;
  (void) memset(dhsResponse,'\0',MAXMSG);
  dhsCloseExp(&dhsLstat,dhsResponse,dhsID,expID);
  if ( STATUS_BAD(dhsLstat) ) {
    (void) sprintf(dhsResult,"%s",dhsResponse);
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* return result */  
  (void) sprintf(dhsResult,"%ld",dhsLstat);
  (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
  return TCL_OK;
}

/*******************************************************************************
 * dhsOpenConnectTcl ( ... )
 *  Use: set dhsId [dhs::OpenConnect <systemID> <{configList}>]
 *******************************************************************************/
static int dhsOpenConnectTcl ( ClientData clientData, Tcl_Interp *interp, int argc, char *argv[] ) {
  /* initialize variables */
  dhsHandle dhsID = (dhsHandle)0;
  fpConfig_t fpConfigTcl = {0L};
  int ierror = DHS_TCL_FALSE;
  int whoami = 0;
  int lsArgc = 0;
  char **lsArgv = (char **)NULL;
  /* check systemID */
  if ( Tcl_GetInt(interp,argv[1],&whoami) != TCL_OK ) {
    (void) sprintf(dhsResult,"%s","dhsOpenConnectTcl-E-bad system id\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* check configuration list */
  if ( Tcl_SplitList(interp,argv[2],&lsArgc,&lsArgv)!=TCL_OK || lsArgc<5L ) {
    (void) sprintf(dhsResult,"%s","dhsOpenConnectTcl-E-bad list\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* set configuration */
  (void) memset((void *)&fpConfigTcl,0,sizeof(fpConfig_t));
  if ( Tcl_GetInt(interp,lsArgv[0],(int *)&(fpConfigTcl.xSize))    != TCL_OK ) ierror = DHS_TCL_TRUE;
  if ( Tcl_GetInt(interp,lsArgv[1],(int *)&(fpConfigTcl.ySize))    != TCL_OK ) ierror = DHS_TCL_TRUE;
  if ( Tcl_GetInt(interp,lsArgv[2],(int *)&(fpConfigTcl.xStart))   != TCL_OK ) ierror = DHS_TCL_TRUE;
  if ( Tcl_GetInt(interp,lsArgv[3],(int *)&(fpConfigTcl.yStart))   != TCL_OK ) ierror = DHS_TCL_TRUE;
  if ( Tcl_GetInt(interp,lsArgv[4],(int *)&(fpConfigTcl.dataType)) != TCL_OK ) ierror = DHS_TCL_TRUE;
  if ( ierror ) {
    (void) sprintf(dhsResult,"%s","dhsOpenConnectTcl-E-bad list element\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* execute the dhs function */
  dhsLstat = 0L;
  (void) memset(dhsResponse,'\0',MAXMSG);
  dhsOpenConnect(&dhsLstat,dhsResponse,&dhsID,(ulong)whoami,&fpConfigTcl);
  if ( STATUS_BAD(dhsLstat) ) {
    (void) sprintf(dhsResult,"%s",dhsResponse);
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* return result */
  (void) sprintf(dhsResult,"%d",(int)dhsID);
  (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
  (void) Tcl_Free((char *)lsArgv);
  return TCL_OK;
}

/*******************************************************************************
 * dhsCloseConnectTcl ( ... )
 *  Use: set dhsStatus [dhs::CloseConnect <dhsID>]
 *******************************************************************************/
static int dhsCloseConnectTcl ( ClientData clientData, Tcl_Interp *interp, int argc, char *argv[] ) {
  /* initialize variables */
  dhsHandle dhsID = (dhsHandle)0;
  /* check handle */
  if ( Tcl_GetInt(interp,argv[1],(int *)&dhsID) != TCL_OK ) {
    (void) sprintf(dhsResult,"%s","dhsCloseConnectTcl-E-bad handle\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* execute the dhs function */
  dhsLstat = 0L;
  (void) memset(dhsResponse,'\0',MAXMSG);
  dhsCloseConnect(&dhsLstat,dhsResponse,dhsID);
  if ( STATUS_BAD(dhsLstat) ) {
    (void) sprintf(dhsResult,"%s",dhsResponse);
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* return result */  
  (void) sprintf(dhsResult,"%ld",dhsLstat);
  (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
  return TCL_OK;
}

/*******************************************************************************
 * dhsSysOpenTcl ( ... )
 *  Use: set dhsId [dhs::SysOpen <systemID>]
 *******************************************************************************/
static int dhsSysOpenTcl ( ClientData clientData, Tcl_Interp *interp, int argc, char *argv[] ) {
  /* initialize variables */
  dhsHandle dhsID = (dhsHandle)0;
  int whoami = 0;
  /* check systemID */
  if ( Tcl_GetInt(interp,argv[1],&whoami) != TCL_OK ) {
    (void) sprintf(dhsResult,"%s","dhsSysOpenTcl-E-bad system id\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* execute the dhs function */
  dhsLstat = 0L;
  (void) memset(dhsResponse,'\0',MAXMSG);
  dhsSysOpen(&dhsLstat,dhsResponse,&dhsID,(ulong)whoami);
  if ( STATUS_BAD(dhsLstat) ) {
    (void) sprintf(dhsResult,"%s",dhsResponse);
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* return result */  
  (void) sprintf(dhsResult,"%d",(int)dhsID);
  (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
  return TCL_OK;
}

/*******************************************************************************
 * dhsSysCloseTcl ( ... )
 *  Use: set dhsStatus [dhs::SysClose <dhsID>]
 *******************************************************************************/
static int dhsSysCloseTcl ( ClientData clientData, Tcl_Interp *interp, int argc, char *argv[] ) {
  /* initialize variables */
  dhsHandle dhsID = (dhsHandle)0;
  /* check handle */
  if ( Tcl_GetInt(interp,argv[1],(int *)&dhsID) != TCL_OK ) {
    (void) sprintf(dhsResult,"%s","dhsSysCloseTcl-E-bad handle\n");
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* execute the dhs function */
  dhsLstat = 0L;
  (void) memset(dhsResponse,'\0',MAXMSG);
  dhsSysClose(&dhsLstat,dhsResponse,dhsID);
  if ( STATUS_BAD(dhsLstat) ) {
    (void) sprintf(dhsResult,"%s",dhsResponse);
    (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
    return TCL_ERROR;
  }
  /* return result */  
  (void) sprintf(dhsResult,"%ld",dhsLstat);
  (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
  return TCL_OK;
}

/*******************************************************************************
 * dhstcl_Init ( ... )
 *******************************************************************************/
int Dhstcl_Init ( Tcl_Interp *interp ) {
 /* initialize any variable(s) and/or interface(s) */
 (void) memset((void *)&dhsResult,'\0',DHS_TCL_RESULT_LEN);
 (void) memset((void *)&dhsHelp,'\0',DHS_TCL_HELP_LEN);
 if ( Tcl_InitStubs(interp,"8.1",0) == (char *)NULL ) return TCL_ERROR;
 /* make the wrapper(s) available to Tcl/tk */
 Tcl_CreateCommand(interp,"dhs::help",dhsHelpTcl,(ClientData)NULL,(Tcl_CmdDeleteProc *)NULL);
 Tcl_CreateCommand(interp,"dhs::SysOpen",dhsSysOpenTcl,(ClientData)NULL,(Tcl_CmdDeleteProc *)NULL);
 Tcl_CreateCommand(interp,"dhs::SysClose",dhsSysCloseTcl,(ClientData)NULL,(Tcl_CmdDeleteProc *)NULL);
 Tcl_CreateCommand(interp,"dhs::OpenConnect",dhsOpenConnectTcl,(ClientData)NULL,(Tcl_CmdDeleteProc *)NULL);
 Tcl_CreateCommand(interp,"dhs::CloseConnect",dhsCloseConnectTcl,(ClientData)NULL,(Tcl_CmdDeleteProc *)NULL);
 Tcl_CreateCommand(interp,"dhs::OpenExp",dhsOpenExpTcl,(ClientData)NULL,(Tcl_CmdDeleteProc *)NULL);
 Tcl_CreateCommand(interp,"dhs::CloseExp",dhsCloseExpTcl,(ClientData)NULL,(Tcl_CmdDeleteProc *)NULL);
 Tcl_CreateCommand(interp,"dhs::IOctl",dhsIOctlTcl,(ClientData)NULL,(Tcl_CmdDeleteProc *)NULL);
 Tcl_CreateCommand(interp,"dhs::ReadImage",dhsReadImageTcl,(ClientData)NULL,(Tcl_CmdDeleteProc *)NULL);
 Tcl_CreateCommand(interp,"dhs::PixelData",dhsPixelDataTcl,(ClientData)NULL,(Tcl_CmdDeleteProc *)NULL);
 Tcl_CreateCommand(interp,"dhs::MetaData",dhsMetaDataTcl,(ClientData)NULL,(Tcl_CmdDeleteProc *)NULL);
 /* provide the package */
 (void) sprintf(dhsResult,"DHS Package v%s, %s, %s",_dhsTcl_S_,_dhsTcl_A_,_dhsTcl_C_);
 (void) Tcl_SetResult(interp,dhsResult,TCL_STATIC);
 return Tcl_PkgProvide(interp,"dhs",_dhsTcl_S_);
}
