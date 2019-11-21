/*******************************************************************************
 * include(s):
 *******************************************************************************/
#if !defined(_semTest_H_)
 #include "semTest.h"
#endif

#if !defined(_semUtil_H_)
 #include "semUtil.h"
#endif

#define PNAME        "semTest"
#define AUTHOR       "Author(s): Philip N. Daly"
#define DATE         "Date: 18-Feb-2004"
#define VERSION      "1.0.1"

int dlibDebug;

/*******************************************************************************
 * command(s):
 *******************************************************************************/
#define CMD_NEW      0x40
#define CMD_INIT     0x41
#define CMD_GIVE     0x42
#define CMD_TAKE     0x43
#define CMD_RELEASE  0x44
#define CMD_DUMP     0x45
#define CMD_QUIT     0x46
#define CMD_EXIT     0x47
#define CMD_HELP     0x48
#define CMD_QUESTION 0x49
#define CMD_UNKNOWN  0x50
#define CMD_GET      0x51
#define CMD_GETVAL   0x52

/*******************************************************************************
 * structure(s):
 *******************************************************************************/
typedef struct {
 #define CMD_NAMELEN 0x10
 #define CMD_HELPLEN 0x50
 char name[CMD_NAMELEN];
 char help[CMD_HELPLEN];
 unsigned char code;
} CMD_ST;
 
static CMD_ST knownCommands[] = {
 {"new",      "Create a new sempahore",              CMD_NEW},
 {"get",      "Get or create a sempahore",           CMD_GET},
 {"getval",   "Get value of sempahore",              CMD_GETVAL},
 {"setval",   "Set value of sempahore",              CMD_INIT},
 {"init",     "Initialize a semaphore with a value", CMD_INIT},
 {"give",     "Give the semaphore",                  CMD_GIVE},
 {"take",     "Take the semaphore",                  CMD_TAKE},
 {"release",  "Release the semaphore",               CMD_RELEASE},
 {"dump",     "Dump system information",             CMD_DUMP},
 {"quit",     "Quit the application",                CMD_QUIT},
 {"exit",     "Exit the application",                CMD_EXIT},
 {"help",     "Invoke minimal help text",            CMD_HELP},
 {"?",        "Invoke minimal help text",            CMD_QUESTION},
 {"",         "",                                    CMD_UNKNOWN},
};

/*******************************************************************************
 * checkCommand ( ... )
 *******************************************************************************/
CMD_ST *checkCommand( unsigned char *inCmd ) {
 CMD_ST *_testCmd = knownCommands;
 unsigned char *_testStr = inCmd;
 while ( *inCmd != 0 ) { *inCmd = tolower(*inCmd); inCmd++; }
 while ( _testCmd->code != CMD_UNKNOWN ) {
  if ( ! strcmp((const char *)_testStr,_testCmd->name) ) {
   break;
  } else {
   _testCmd++;
  }
 }
 return _testCmd;
}

/*******************************************************************************
 * parseCommand ( ... )
 *******************************************************************************/
void parseCommand( unsigned char *inCmd ) {

 /* declare some variables and initialize them */
 char _testMsg[MAXMSG]={'\0'};
 char _testFName[MAXMSG]={'\0'};
 char *_testPtr = &_testMsg[0];
 CMD_ST *_testCmd = checkCommand(inCmd);
 long _testStat = 0L;
 int _testVal = 0L;
 int _testSid = 0;
 int _testAccess = 0;

 /* test each case */
 switch ( _testCmd->code ) {

  case CMD_GET:
   (void) printf("\n%s _testFName > ",PNAME);
   (void) scanf("%s",_testFName);
   (void) printf("\n%s _testAccess > ",PNAME);
   (void) scanf("%d",&_testAccess);
   semUtilGet(&_testStat,_testPtr,(char *)_testFName,&_testSid,_testAccess);
   if ( _testPtr != (char *) NULL ) (void) printf("%s\n",_testPtr);
   if ( _testStat == OK ) { (void) printf("%s: semaphore gotten with id=%d\n",PNAME,_testSid); (void) fflush(stdout); }
   break;

  case CMD_NEW:
   (void) printf("\n%s _testAccess > ",PNAME);
   (void) scanf("%d",&_testAccess);
   semUtilNew(&_testStat,_testPtr,&_testSid,_testAccess);
   if ( _testPtr != (char *) NULL ) (void) printf("%s\n",_testPtr);
   if ( _testStat == OK ) { (void) printf("%s: semaphore created with id=%d\n",PNAME,_testSid); (void) fflush(stdout); }
   break;

  case CMD_RELEASE:
   (void) printf("\n%s _testSid > ",PNAME);
   (void) scanf("%d",&_testSid);
   semUtilRelease(&_testStat,_testPtr,_testSid);
   if ( _testPtr != (char *) NULL ) (void) printf("%s\n",_testPtr);
   break;

  case CMD_INIT:
   (void) printf("\n%s _testSid > ",PNAME);
   (void) scanf("%d",&_testSid);
   (void) printf("\n%s _testVal > ",PNAME);
   (void) scanf("%d",&_testVal);
   semUtilInit(&_testStat,_testPtr,_testSid,_testVal);
   if ( _testPtr != (char *) NULL ) (void) printf("%s\n",_testPtr);
   break;

  case CMD_GETVAL:
   (void) printf("\n%s _testSid > ",PNAME);
   (void) scanf("%d",&_testSid);
   semUtilGetVal(&_testStat,_testPtr,_testSid,&_testVal);
   if ( _testPtr != (char *) NULL ) (void) printf("%s\n",_testPtr);
   if ( _testStat == OK ) { (void) printf("%s: semaphore value=%d\n",PNAME,_testVal); (void) fflush(stdout); }
   break;

  case CMD_GIVE:
   (void) printf("\n%s _testSid > ",PNAME);
   (void) scanf("%d",&_testSid);
   semUtilGive(&_testStat,_testPtr,_testSid);
   if ( _testPtr != (char *) NULL ) (void) printf("%s\n",_testPtr);
   break;

  case CMD_TAKE:
   (void) printf("\n%s _testSid > ",PNAME);
   (void) scanf("%d",&_testSid);
   semUtilTake(&_testStat,_testPtr,_testSid);
   if ( _testPtr != (char *) NULL ) (void) printf("%s\n",_testPtr);
   break;

  case CMD_DUMP:
   (void) system("/bin/cat /proc/sysvipc/sem");
   break;

  case CMD_EXIT: case CMD_QUIT:
   exit(0);
   break;

  case CMD_HELP: case CMD_QUESTION:
   _testCmd = knownCommands;
   while ( _testCmd->code != CMD_UNKNOWN ) {
     (void) printf("%s\t\t - %s\n",_testCmd->name,_testCmd->help);
     _testCmd++;
   }
   break;

  case CMD_UNKNOWN: default:
   (void) system((const char *)inCmd);
   break;
 }
 (void) fflush(stdout);
}

/*******************************************************************************
 * main ( ... )
 *******************************************************************************/
int main ( int argc, char *argv[] ) {
 /* declare some variables and initialize them */
 unsigned char _testLine[CMD_NAMELEN];
 /* get command line arguments */
 while ( argc>1 && argv[1][0]=='-' ) {
  switch (argv[1][1]) {
   case 'h': case 'H': case 'v': case 'V': case '?': default:
    (void) fprintf(stdout,"%s: v%s %s %s\n",PNAME,VERSION,AUTHOR,DATE);
    return fprintf(stdout,"Use: %s [-h -v]\n",PNAME);
    break;
  }
  argc--; argv++;
 }
 (void) fprintf(stdout,"%s: v%s %s %s\n",PNAME,VERSION,AUTHOR,DATE);
 (void) fflush(stdout);
 /* loop forever */
 for (;;) {
  /* reset the command line */
  (void) memset(_testLine,'\0',CMD_NAMELEN);
  /* print out a prompt */
  (void) printf("\n%s Test Command (?) > ",PNAME);
  /* read the command line */
  (void) scanf("%s",_testLine);
  /* parse the command line */
  parseCommand(_testLine);
 }
 /* return normally */ 
 return 0;
}
