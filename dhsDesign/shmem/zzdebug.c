#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <errno.h>


extern key_t shmUtilInit();
extern void *shmUtilAttach();

main (int argc, char **argv)
{
    int new 	 = 0;
    int create 	 = 1;
    long size 	 = 1024;
    void *addr 	 = NULL;
    key_t memKey = 0;
    char lockfile[128];


    if (memKey = shmUtilInit (&new, lockfile)) {
	printf ("INIT: memKey=%ld  lockfile='%s' new=%d\n",
	    memKey, lockfile, new);

        if (addr = shmUtilAttach (memKey, new, size, &create))
	    printf ("ATTACH: addr=%ld  create=%ld\n", addr, create);

system("cat /proc/sysvipc/shm");
printf ("\n\n"); sleep (3);


	shmUtilDetach (memKey, addr, 1);
    }

    printf ("Done.\n");
system("cat /proc/sysvipc/shm");
}
