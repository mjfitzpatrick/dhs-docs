/*******************************************************************************
 * include(s):
 *******************************************************************************/
#if !defined(_shmTest_H_)
#include "shmTest.h"
#endif

#if !defined(_shmUtil_H_)
#include "shmUtil.h"
#endif

#define PNAME        "shmTest"
#define AUTHOR       "Author(s): Philip N. Daly"
#define DATE         "Date: 12-Nov-2002"
#define VERSION      "1.0.0"

int dlibDebug;
char __simStr[10] = "";

/*******************************************************************************
 * command(s):
 *******************************************************************************/
#define CMD_ATTACH   0x40
#define CMD_DETACH   0x42
#define CMD_EXTRACT  0x53
#define CMD_INIT     0x43
#define CMD_SIZE     0x44
#define CMD_UNINIT   0x46
#define CMD_QUIT     0x47
#define CMD_EXIT     0x48
#define CMD_HELP     0x49
#define CMD_QUESTION 0x50
#define CMD_UNKNOWN  0x51
#define CMD_DUMP     0x52

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
    {"attach", "get a shared memory segment address", CMD_ATTACH},
    {"detach", "Detach from shared memory", CMD_DETACH},
    {"extract", "Extract data from shared memory", CMD_EXTRACT},
    {"size", "Resize an area", CMD_SIZE},
    {"init", "Init a shared memory segment", CMD_INIT},
    {"uninit", "Un-init a shared memory segment", CMD_UNINIT},
    {"dump", "Dump the shared memory info", CMD_DUMP},
    {"quit", "Quit the application", CMD_QUIT},
    {"exit", "Exit the application", CMD_EXIT},
    {"help", "Invoke minimal help text", CMD_HELP},
    {"?", "Invoke minimal help text", CMD_QUESTION},
    {"", "", CMD_UNKNOWN},
};

/*******************************************************************************
 * checkCommand ( ... )
 *******************************************************************************/
CMD_ST *checkCommand(unsigned char *inCmd)
{
    CMD_ST *_testCmd = knownCommands;
    unsigned char *_testStr = inCmd;
    while (*inCmd != 0) {
	*inCmd = tolower(*inCmd);
	inCmd++;
    }
    while (_testCmd->code != CMD_UNKNOWN) {
	if (!strcmp((const char *) _testStr, _testCmd->name)) {
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
void parseCommand(unsigned char *inCmd)
{

    /* declare some variables and initialize them */
    static ulong *_testBuffer = (ulong *) NULL;
    ulong *_testData = (ulong *) NULL;
    char _testStr[MAXMSG] = { '\0' };
    char _testMsg[MAXMSG] = { '\0' };
    char *_testPtr = &_testMsg[0];
    CMD_ST *_testCmd = checkCommand(inCmd);
    long _testStat = 0L;
    int _testStart = 0;
    int _testEnd = 0;
    int _testInit = 0;
    ulong _testVal = 0;
    ulong _testUnits = 0;
    int _testCreate = TRUE;
// long _testId = 0L | IPC_CREAT | S_IRUSR | S_IWUSR;

    /* test each case */
    switch (_testCmd->code) {

    case CMD_ATTACH:
	printf("\n%s _testLibName > ", PNAME);
	scanf("%s", _testStr);
	printf("\n%s _testSize > ", PNAME);
	scanf("%ld", &_testVal);
	_testBuffer =
	    (ulong *) shmUtilAttach(&_testStat, _testPtr, _testStr,
				    &_testCreate, _testVal);
	if (_testPtr != (char *) NULL)
	    printf("ptr = %s\n", _testPtr);
	printf("%s: address = %p\n", PNAME, _testBuffer);
	fflush(stdout);
	break;

    case CMD_DETACH:
	printf("\n%s _testLibName > ", PNAME);
	scanf("%s", _testStr);
	printf("%s: address = %p\n", PNAME, _testBuffer);
	fflush(stdout);
	shmUtilDetach(&_testStat, _testPtr, _testStr,
		      (void *) _testBuffer);
	if (_testPtr != (char *) NULL)
	    printf("%s\n", _testPtr);
	break;

    case CMD_EXTRACT:
	printf("\n%s _testStart > ", PNAME);
	scanf("%d", &_testStart);
	printf("\n%s _testEnd > ", PNAME);
	scanf("%d", &_testEnd);
	printf("%s: address=%p, start=%d, end=%d\n", PNAME,
		      _testBuffer, _testStart, _testEnd);
	fflush(stdout);
	for (_testData = _testBuffer + _testStart, _testVal = 0;
	     _testVal < (_testEnd - _testStart + 1);
	     _testVal++, _testData++)
	    printf("%s: address=%p, value[%ld]=%ld\n", PNAME,
			  _testData, _testVal, (ulong) * (_testData));
	fflush(stdout);
	break;

    case CMD_INIT:
	printf("\n%s _testLibName > ", PNAME);
	scanf("%s", _testStr);
	printf("\n%s _testNew > ", PNAME);
	_testVal =
	    (ulong) shmUtilInit(&_testStat, _testPtr, _testStr,
				&_testInit);
	printf("%s: memKey = %ld\n", PNAME, _testVal);
	fflush(stdout);
	break;

    case CMD_SIZE:
	printf("\n%s _testBytes > ", PNAME);
	scanf("%ld", &_testVal);
	printf("\n%s _testUnits > ", PNAME);
	scanf("%ld", &_testUnits);
	shmUtilSize(&_testStat, _testPtr, &_testVal, _testUnits);
	if (_testPtr != (char *) NULL)
	    printf("%s\n", _testPtr);
	printf("%s: _testVal = %ld\n", PNAME, _testVal);
	break;

    case CMD_UNINIT:
	printf("\n%s _testLibName > ", PNAME);
	scanf("%s", _testStr);
	shmUtilUninit(&_testStat, _testPtr, _testStr);
	if (_testPtr != (char *) NULL)
	    printf("%s\n", _testPtr);
	break;

    case CMD_DUMP:
	system("cat /proc/sysvipc/shm");
	break;

    case CMD_EXIT:
    case CMD_QUIT:
	exit(0);
	break;

    case CMD_HELP:
    case CMD_QUESTION:
	_testCmd = knownCommands;
	while (_testCmd->code != CMD_UNKNOWN) {
	    printf("%s\t\t - %s\n", _testCmd->name, _testCmd->help);
	    _testCmd++;
	}
	break;

    case CMD_UNKNOWN:
    default:
	system((const char *) inCmd);
	break;
    }
    fflush(stdout);
}


/*******************************************************************************
 * main ( ... )
 *******************************************************************************/
int main(int argc, char *argv[])
{
    /* declare some variables and initialize them */
    unsigned char _testLine[CMD_NAMELEN];


    /* get command line arguments */
    while (argc > 1 && argv[1][0] == '-') {
	switch (argv[1][1]) {
	case 'h':
	case '?':
	default:
	    fprintf(stdout, "%s: v%s %s %s\n", PNAME, VERSION,
			   AUTHOR, DATE);
	    return fprintf(stdout, "Use: %s [-h -v]\n", PNAME);
	    break;
	}
	argc--;
	argv++;
    }
    fprintf(stdout, "%s: v%s %s %s\n", PNAME, VERSION, AUTHOR, DATE);
    fflush(stdout);


    /* loop forever */
    for (;;) {
	memset(_testLine, '\0', CMD_NAMELEN); 	/* reset the command line */
	printf("\n%s Cmd (?) > ", PNAME);	/* print out a prompt */
	scanf("%s", _testLine); 		/* read the cmdline */
	parseCommand(_testLine); 		/* parse the cmdline */
    }

    return 0; 					/* return normally */
}
