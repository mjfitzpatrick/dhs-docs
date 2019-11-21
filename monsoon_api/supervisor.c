/* Supervisor code for NEWFIRM DHS                                    */
/* Author: Michelle Miller                                            */
/* Date:  Fri Apr  8 14:53:54 MST 2005                                */

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
#include "proto.h"
#include "farm.h"

int startCollector (char*,int);
int registerCollector (char *,char *, int);
/*int assignRequest(int);
int reassignRequest(int);
int updateMachines(); */

Farm farm;   /* all machines in grid */

/* read in port from a config file */
int 
main(int argc, char **argv)
{
   int listen_sock, pid, client, from_length, port=0;
   struct sockaddr_in host_addr, from;
   struct hostent *hp;
   char *fname = NULL;
   char *buf;
   char *token;
   int numInstances, status, sockbufsize=LINE_SZ;
   int write_cnt;
   msgType tag;
   char *response=NULL;
   char *collector_name;
   int collector_port;
   char **endptr;
   unsigned long caller;

   if (argc == 3)  {
      fname = strdup (argv[1]);
      port = atoi(argv[2]);
   }
   else if (argc > 3) {
      fprintf (stderr, "USAGE: server {out_file} {port}\n");
      exit(-1);
   }
                                                                                
fprintf (stderr, "entering server\n");
   if (!port)
      port = 9099;

   /* setup listening socket at known port */
   listen_sock = socket (AF_INET, SOCK_STREAM, 0);
   if (listen_sock == -1) {
      perror ("open socket error");
      exit(-1);
   }

   /* bind and fill in sockaddr_in with server's well-known port */
   host_addr.sin_family = AF_INET;
   host_addr.sin_port = htons(port);
   host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

   if (bind (listen_sock, (struct sockaddr *)&host_addr,
                                      sizeof (host_addr)) < 0) {
      perror("bind error");
      exit (-1);
   }
   if (listen (listen_sock, 5) < 0) {
      perror ("listen error");
      exit (-1);
   }

   (void *)buf = malloc (sockbufsize);
   if (buf == NULL) {
      perror ("malloc error");
      exit(-1);
   }

   /* accept incoming connection and fork to process */
   for (;;) {
      client = accept(listen_sock, (struct sockaddr *)&from,
                      (socklen_t *)&from_length);
      if (client < 0) {
         if (errno == EINTR)
            continue;                  /* go back to for() */
         else {
            perror("accept failed");
            exit(-1);
         }
      } else {
fprintf (stderr, "accept succeeded\n");
         if ((pid = fork()) == 0) {  /* child */
            close (listen_sock);
            /* easiest if sockbufsize is same with every type of call: pad */
            if (read (client, buf, sockbufsize) == -1) {
               perror ("supervisor main() fails read client socket");
               close (client);
               exit(-1);
            }
            token = strtok (buf, ":");
            (int)tag = atoi (token);
            switch (tag) {
               case SYSOPEN:
                  token = strtok (NULL, "\0");
                  numInstances = atoi (token);
                  status = startCollector(response, numInstances);
                  break;
/*
               case OPENCONNECT: 
                  token = strtol (NULL, "\0");
                  caller = strtoul (token, endptr,16);
                  status = assignRequest(response, caller);
                  break;
               /* case RECONNECT:
                  reassignRequest();
               case HEARTBEAT:
                  updateCollector();
*/
               case REGISTER:
                  token = strtok (NULL, ":");
                  collector_name = strdup (token);
                  token = strtok (NULL, "\0");
                  port = atoi(token);
                  status = registerCollector (response, collector_name, port);
                  break;
               default:
                  break;  
            }
            bzero(buf, sizeof(buf));
            sprintf (buf, "%d", status);
            if (status == ERROR)
            {
               strcat (buf, ":");
               strcat (buf, response);
            }
            write_cnt = write (client, buf, strlen(buf));
fprintf (stderr, "send buffer size = %d, wrote %d bytes\n",
         strlen(buf), write_cnt);
            if (write_cnt < 0) {
               perror("supervisor failed ACK write");
               /* not sure anything can be done here.. */
            }
            exit(0);
         }  /* end child */
         else       /* parent closes client socket being serviced by child */
         {
            close (client); 
/* ZZZ- do next, also need sighandler (see Stevens) */
            //wait4(
         }
      } /* end accept success */
   }  /* end for */
}

/* either send request for additional information to socket or get it all *
 * during first send() from Collector.  Which is simpler?                 */
int
registerCollector(char *response, char *collector_name, int collector_port)
{
   Machine *m;

   m = farm.findMachine(collector_name);
   if (m == NULL) {
      fprintf (stderr, "No matching machine %s in farm\n", collector_name);
      response = strdup ("No matching machine in farm");
      return ERROR;
   }
   /* calculate speed or get it from /proc/cpuinfo on Collector? */
   /* set registered flag */
   m->registerMachine();
   m->listen_port = collector_port;
   return SUCCESS;
}

/* startCollector (response, numInstances)                                  *
 *                                                                          *
 * Read configuration file to locate potential machines for Collectors to   *
 * run on.  Start at top of list and work down, starting one Collector per  *
 * machine until numInstances Collectors have been started.  No config.     *
 * file or one that cannot be opened will result in numInstances            *
 * Collectors being started on the localhost.  The default numInstances is  *
 * two. Read scheduling criteria here also.                                 */
int
startCollector (char *response, int numInstances)
{
   FILE *fp;
   char *machine_name;
   char command[LINE_SZ];
   char *str_procs;
   char my_name[MAX_HOSTNAME];
   char line[LINE_SZ];
   Machine *m;

fprintf (stderr, "Entering supervisor::startCollector()\n");
   /* get machines to use from config file and store in Farm */
   fp = fopen ("dhs_config", "r");
   if (fp == NULL)
   {
      fprintf(stderr,"Cannot open dhs_config: using default configuration\n");
      machine_name = strdup ("localhost");
      m = new Machine(machine_name);
      farm.addMachine(*m);
      delete(m);
   } else {
      while (fgets (line, LINE_SZ, fp) != NULL) /* returns NULL on EOF */
      {
fprintf (stderr, "line of dhs_config = %s\n", line);
         machine_name = strtok (line, ":");   
         str_procs = strtok (NULL, "\0");  
         m = new Machine(machine_name);
         m->processors = atoi(str_procs); /* can also get from system file */
m->printMachine();
         farm.addMachine(*m);
fprintf (stderr, "add Machine %s to farm\n", machine_name);
farm.printFarm(); 
         /* farm.rankMachines (scheduleParameters); */
         delete(m);
         bzero(line, LINE_SZ);
      }
      fclose(fp);
   }

   /* is supervisor machine in farm? */
   if (gethostname (my_name, MAX_HOSTNAME) == ERROR) {
      perror ("supervisor StartCollector() gethostname error");
   } else {
      m = farm.findMachine(my_name);   
      if (m ==NULL)
      { 
         fprintf (stderr, "Supervisor machine %s not in farm\n", my_name);
         m = new Machine(my_name);  /* do I really want to do this? */
      }
      fprintf (stderr, "Supervisor is running on %s\n", m->getName());
   }
   
/* startup mechanisms: rsh/ssh, log into machine and startup, daemon (crontab)
 *    xinetd - Collector will need command-line startup capability */
/* daemon started from crontab would already be up and connected to superv. */
/* log into machine and startup too cumbersome, but sidesteps passwd problem */
/* ssh requires a passwd unless tunneling set up */
/* inetd startup requires root to configure */

   /* Collector on remote machine must be in user's $PATH on that machine.   */
   /* Start Collector on first machine in Farm, moving down list until       *
    * numInstances requested have been started. */
   for (int i=0; i < numInstances; i++)
   {
      bzero (command, sizeof(command));
      strcpy (command, "ssh ");
      m = farm.getMachine(i);
      strcat (command, m->getName());
      strcat (command, " collector\n");
      /* how do I detect that an ssh failed? */
fprintf (stderr, "command = %s", command);
      if (system (command) == -1)
      {
           response = strdup ("fork failed");
           return (ERROR);
      }
   }
   return (SUCCESS);
}

/* simpler if I just choose one machine for the calling client.  Client can  *
 * request another machine if there is a problem.  But how will I know       *
 * there was a problem connecting to the machine? 
 * This assignment will be a permanent connection between client (PAN/NOCS)  *
 * for the duration of a sequence (observation set).  */

/* Are scheduling heuristics fixed for a run or will they change in middle? */
/*
int
assignRequest(char *response, unsigned long caller, Machine* chosen)
{
   /* assume machine list already sorted */
/*
   if (caller == DHS_IAMPAN) {
      chosen = ranked[0];
      chosen->clients++;
      farm.rankMachines();
   } else {
      chosen = this_machine;
   }
     
/* mark reservation in list & keep track of client request - obs set tracking */
/* return top choice */
/*
}

*/
/* Should supervisor check with machine that failed to service client       *
 * request? Either supervisor can detect failure or client can notify       *
 * supervisor. What if previously assigned machine processed a subset of    *
 * the observation set before going down?  How do we get that back?         */
/*int
reassignRequest(int scheduleParameters)
{
/* scan machine list using scheduling criteria specified & eliminating
machine previously assigned */
/* rank machine choices */
/* mark reservation in list & keep track of client request - obs set tracking */
/* return top choice */
/*
}
*/

/* add new machine to list or update workload of a machine already in list */
/* int 
updateMachines(char *machine_name, int workload, int cpu_speed)
{
/* search for machine name entry in machine list */
/* if found, update workload and pending requests */
/* else add new entry */
/* ACK? */
/*
}
*/
