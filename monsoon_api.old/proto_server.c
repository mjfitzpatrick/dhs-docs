/* Server program for DHS transfer of pixel data  	*/
/* Author: Michelle Miller			  	*/
/* Date: Wed Jul 28 13:18:52 MST 2004			*/

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include "proto.h"

#define SOCKBUFSZ 96000

extern double cput, realt;

int 
main (int argc, char **argv)
{
   int recv_sock, fd, port=0;
   struct sockaddr_in host_addr, from;
   struct hostent *hp;
   int client, from_length;
   long n=0;
   long cnt=0;
   long sockbufsize;
   char fname[] = {"mm_test"};
   void *buf;
   char stats[128];
   double nbytes=0;

   if (argc == 2) 
      port = atoi(argv[1]);
   else if (argc > 2) {
      fprintf (stderr, "USAGE: server {port}\n");
      exit(-1);
   }

fprintf (stderr, "entering server\n");
   if (!port) 
      port = 9099;

   host_addr.sin_family = AF_INET;
   host_addr.sin_port = htons(port);
   host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

   recv_sock = socket(AF_INET, SOCK_STREAM, 0);
   if (recv_sock == -1) {
      perror ("open socket error");
      exit(-1);
   }

fprintf (stderr, "socket = %d\n", recv_sock);

   if (bind (recv_sock, (struct sockaddr *)&host_addr, sizeof(host_addr)) < 0) {
      perror("bind error");
      exit(-1);
   }
   if (listen(recv_sock, 5) < 0) {
      perror ("listen error");
      exit(-1);
   }

   /* adjust TCP window size of CONNECTED socket to optimal size found
      experimentally */
   sockbufsize = SOCKBUFSZ;
   if (setsockopt (recv_sock, SOL_SOCKET, SO_RCVBUF, &sockbufsize,
                   sizeof(long)) < 0) {
      perror ("setsockopt error");
      close (recv_sock);
      exit(-1);
   }

   buf = malloc (sockbufsize);
   if (buf == NULL) {
      perror ("malloc error");
      exit(-1);
   }

fprintf (stderr, "entering infinite loop\n");
   for (;;)
   {
      client= accept(recv_sock, (struct sockaddr *)&from, &from_length);
      bzero (buf, sockbufsize);
      nbytes = 0;
      if (client < 0) {
         perror("accept failed");
         exit(-1);
      } else {
         fprintf (stderr, "accept succeeded\n");
         /* open file to write data from socket into */
         fd = open (fname, O_CREAT|O_WRONLY|O_APPEND, S_IRUSR|S_IWUSR);
         if (fd == -1) {
            perror ("file open failed");
            /* *status = -1; send status back to client? */
            exit(-1);
         }

         /* just read into a buffer  - need a while loop */
         prep_timer();
         while ((n = read (client, buf, sockbufsize)) > 0)
         {  
            cnt = write (fd, buf, n);
            if (cnt < 0) {
               perror ("write to file failed");
               close (fd);
               close (client);
               exit(-1);
            }
            nbytes += n;
            bzero (buf, sockbufsize); 
         }
         read_timer(stats,sizeof(stats));
fprintf (stderr, "%.0f bytes in %.4f real seconds = %.2f bytes/sec\n", nbytes,
         realt, (nbytes/realt));
         close (fd);
      }  /*end of else */
      close(client);
   } /* end of for */
   close (recv_sock);
   exit(0);
}
