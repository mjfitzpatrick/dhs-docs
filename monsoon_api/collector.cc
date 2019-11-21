/* Collector class for NEWFIRM DHS                                        */
/* Author: Michelle Miller                                                */
/* Date: Thu Mar 31 14:07:28 MST 2005                                     */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <syslog.h>
#include <signal.h>

#include <string>
using namespace std;

#include "proto.h"

#define MAXFD 64
/* ZZZ- disable interrupts or at least check for them*/

int connectTo (char *, int);

class Collector {
public:
   Collector();
   ~Collector();
   /* sendHeartbeat(); */
   void getPixelData(); 
   /* getMetadata(); */
   /* compareMetadata(); */
   void init();
   void execute();
   void processMessages();
private:
/*
  Port iport;
  Port oport; */
  char machine_name[MAX_HOSTNAME];
  int workload;            /* uptime */
  int cpu_speed; 	   /* run linpack in constructor */
  int listen_sock;         /* listen for clients */
  int client;              /* connection to client - child */
  int supervisor_sock;     /* socket connected to supervisor - persistent? */
 /* data struct to track whether we have all 4 arrays of exposure - Supervisor? */
/* data struct to track whether we have all exposures in observation set */
};

/* need command-line startup */
/* daemon code from Stevens, vol. 1, p. 336 */
int
main (int argc, char ** argv) 
{
   pid_t pid;

fprintf (stderr, "Collector running.... ");

   /* fork to allow the ssh to return */
   if ((pid = fork()) != 0)   /* parent terminates */
      exit(0);

   setsid();         	      /* 1st child becomes session leader */
   signal (SIGHUP, SIG_IGN);
   if ((pid = fork()) != 0)   /* 1st child terminates */
      exit(0);

   /*daemon_proc = 1;	      /* 2nd child - need? */
/*   chdir ("/tmp");
   umask(0);		      /* clear file mode creation mask */
fprintf (stderr, "still running... ");
   for (int i=0; i < MAXFD; i++)    /* close inherited file descriptors */
      close(i);

   openlog(argv[0], LOG_PID,0);
fprintf (stderr, "and now? ");

   Collector c;
   /* pass in supervisor node:port - collectors talk to different port than clients? */
   c.init();
   c.execute();
}

Collector::Collector()
{
   if (gethostname(machine_name, MAX_HOSTNAME)== -1) {
      perror ("Collector::Collector() gethostname error");
      exit (-1);
   }
}

Collector::~Collector()
{
}

/* setup listening socket */
void Collector::init() 
{
   struct sockaddr_in host_addr;
   socklen_t len;
   char buf[LINE_SZ];
   int sockbufsize=LINE_SZ;
   char tmp[4];
   int write_cnt=0;
   int status;
   char *token;

fprintf (stderr, "Entering Collector::init()\n");
   listen_sock = socket (AF_INET, SOCK_STREAM, 0);
   if (listen_sock == -1) {
      perror ("Collector::init() open socket error");
      exit(-1);
   }

   /* bind and let kernel choose port */
   host_addr.sin_family = AF_INET;
   host_addr.sin_port = htons(0);         
   host_addr.sin_addr.s_addr = htonl(INADDR_ANY);
                                                                                
   if (bind (listen_sock, (struct sockaddr *)&host_addr,
                                      sizeof (host_addr)) < 0) {
      perror("Collector::init() bind error");
      exit (-1);
   }

   if (listen (listen_sock, MAX_QUEUE) < 0) {
      perror ("Collector::init() listen error");
      exit (-1);
   }

   /* find port number assigned by kernel and send to supervisor */
   if (getsockname(listen_sock, (struct sockaddr *)&host_addr, &len) < 0) {
      perror ("Collector::init() getsockname error");
      exit (-1);
   }

   /* just use main listen port on supervisor for now - select? */
   /* msg = "REGISTER:hostname:listening_port" */
   bzero (buf, sizeof(buf));
   sprintf (buf, "%d", REGISTER);
   strcat (buf, ":");
   strcat (buf, machine_name);
   strcat (buf, ":");
   sprintf (tmp, "%d", host_addr.sin_port);
   strcat (buf, tmp);

   /* must open a connection to the supervisor first... */
   char* supervisor = getenv("DHS_SUPERVISOR");
   char* ascii_port = getenv("DHS_PORT");
   int port = atoi (ascii_port);
   supervisor_sock = connectTo (supervisor, port);

   write_cnt = write (supervisor_sock, buf, strlen(buf));
fprintf (stderr, "msg to supervisor = %s\n", buf);
   if (write_cnt < 0) {
      perror ("Collector::init() msg to supervisor failed");
      close (listen_sock);
      close (supervisor_sock);
      exit(-1);  /* or retry? */
   }

   /* get MHz from /proc/cpuinfo - ZZZ what if >1 CPU? */
   /* ZZZ - cannot put this value in a variable to send */
   /* system("awk '/cpu MHz/ {print $4}' /proc/cpuinfo"); */

   /* read return value from remote routine, success? */
   bzero(buf, sizeof(buf));
   if (read (supervisor_sock, buf, sockbufsize) == ERROR) {
      perror ("collector fails read register ACK");
   }
   else if (strlen(buf) > 1) {
      token = strtok (buf, ":");
      status = atoi (token);     
      token = strtok (NULL, "\0");
   } else {
      token = strtok (buf, "\0");
      status = atoi (token);
   }
   if (status == ERROR) {
      fprintf (stderr, "ERROR: %s\n", token);
      close (supervisor_sock);
      exit(-1);
   }
   close (supervisor_sock);
}

int
startExposure()
{
/* establish new connection with client for exposure - one connection per 
 * observation set */
/* record observation set id, total expected exposures, exposure ID and count */
/* create filename from above and open file for writing (separate files for
each array?) */
/* who am I talking to? - getpeername() */
}

void Collector::processMessages()
{
   msgType tag;
   /* fixed size buf or dynamic? */
   char *buf;
   int sockbufsize=LINE_SZ;
   char *token;
   int status;

   /* should "client" be a listening socket too? 
    * what does it mean to be a listening socket? */
   read (client, buf, sockbufsize);
   token = strtok (buf, ":");  /* strtok allocates space */
   (int)tag = atoi (token);
   switch (tag) {
      case STARTEXP:
         status = startExposure();
         /* write (); */
      /* case PIXELDATA:
          status = getPixelData();
      case METADATA:
          status = getMetaData(); */
      default:
          break;
   }
}

/* member functions or independent? */
void Collector::execute()
{
   int from_length;
   struct sockaddr_in from;
   int pid;

fprintf (stderr, "Entering Collector::execute() with listen_sock = %d", 
         listen_sock);
   for (;;) 
   {
      client = accept(listen_sock, (struct sockaddr *)&from, (socklen_t *)&from_length);
      if (client < 0) {
         perror("accept failed");
         exit(-1);
      } else {
         fprintf (stderr, "accept succeeded\n");

         /* need to not have child go back thru infinite loop since this
          * child will have a dedicated connection to one client, no more */
         if ((pid = fork()) == 0) {  /* child */
            close (listen_sock);
            processMessages(); 
         } else {          /* parent */
            close (client);
            /* wait? to eliminate zombie children */
         }
      }  /* end accept */
   }   /* end infinite loop */
}


void Collector::getPixelData()
{
   int sockbufsize;
   char *buf;

   /* adjust TCP window size of CONNECTED socket to optimal size found
      experimentally */
   sockbufsize = SOCKBUFSZ;
   if (setsockopt (client, SOL_SOCKET, SO_RCVBUF, &sockbufsize,
                   sizeof(long)) < 0) {
      perror ("setsockopt error");
      close (client);
      exit(-1);
   }
                                                                                
   (void *)buf = malloc (sockbufsize);
   if (buf == NULL) {
      perror ("malloc error");
      exit(-1);
   }
}
