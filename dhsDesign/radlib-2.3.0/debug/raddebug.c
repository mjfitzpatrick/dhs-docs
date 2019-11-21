#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

#include <radsysdefs.h>
#include <radbuffers.h>
#include <radsemaphores.h>
#include <radsystem.h>

int main (int argc, char *argv[])
{
    int        sysID;

    if (argc < 2)
    {
        printf ("\nYou must specify a valid radlib system ID (1-255)...\n");
        return 1;
    }
    sysID = atoi (argv[1]);

    if (radSystemInit (sysID) == ERROR)
    {
        printf ("\nError: unable to attach to wview radlib system %d!\n",
                sysID);
        return 1;
    }

    printf ("\nAttached to radlib system %d: UP %s\n\n",
            sysID, radSystemGetUpTimeSTR (sysID));

    // dump out the system buffer info
    radBuffersDebug ();
    printf ("\n");

    // dump out semaphore info
    radSemDebug ();

    printf ("\n");

    radSystemExit (sysID);
    return 0;
}
