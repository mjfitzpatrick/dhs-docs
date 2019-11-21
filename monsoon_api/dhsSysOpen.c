/* Implementation of Monsoon/DHS API for NEWFIRM - vers. 4              */
/* Author: Michelle Miller                                              */
/* Date: Thu Mar 31 14:07:28 MST 2005                                   */

#include "proto.h"

/* dhsSysOpen()                                                         *
 *			                                                *
 * Function used by NOCS and Monsoon Supervisor to send DHS supervisor  *
 * information about the overall configuration of the observation and   *
 * focal plane. (mm- no obs info in call.  Send recipe? no focal plane  *
 * info either)                                                         */

/* MM - Function used by NOCS and Monsoon Supervisor to announce the    *
 * start of an observing session. I don't think we need to return a     *
 * dhsHandle here.  What would it be used for? Verify that this can     *
 * only be called once.  No need for different session ids? sysinit()   *
 * The calling side could request a number of DHS nodes to be started.  *
 * Restrict who can call this routine so that it only gets called once  *
 * per instrument per observing session.                                */
/*void
dhsSysOpen (long *status, char *response, dhsHandle *dhsID, ulong whoAmI)
*/

int
dhsSysOpen (long *status, char *response, int numNodes)
{
   char *node;
   int port, dhs_sock;
   int write_cnt=0;
   struct sockaddr_in host_addr;
   struct hostent *hp;
   char buf[LINE_SZ];
   int sockbufsize=LINE_SZ;
   char *ascii_port;

   /* check status?  doesn't really make sense since this is first call. */

   /* find supervisor node */
   node = getenv("DHS_SUPERVISOR");
   if (node == NULL) {
      node = strdup ("localhost");
   }
fprintf (stderr, "DHS_SUPERVISOR = %s\n", node);

   ascii_port = getenv("DHS_PORT");
   if (ascii_port == NULL) {
      port = 9099;  /* default */
   } else {
      port = atoi (ascii_port);
   }
    
   /* socket setup */
   dhs_sock = socket (AF_INET, SOCK_STREAM, 0);
   if (dhs_sock == -1) {
      perror ("open socket error");
      exit(-1);
   }

   /* bzero((char *)&host_addr, sizeof(host_addr));  not needed... */
   hp = gethostbyname (node);
   if (hp == NULL) {
      perror("gethostbyname error");
      close(dhs_sock);
      exit(-1);
   }
   delete (node);
                                                                                
   bcopy(hp->h_addr_list[0],(char *)&host_addr.sin_addr.s_addr,hp->h_length);
   host_addr.sin_family = AF_INET;
   host_addr.sin_port = port;
                                                                                
fprintf (stderr, "socket = %d host_addr setup\n", dhs_sock);

   /* connect */
   if (connect (dhs_sock, (struct sockaddr *)&host_addr, sizeof(host_addr)) == -1) {
      perror("dhsSysOpen() connect failed");
      close(dhs_sock);
      *status = errno;
      /* who allocates space for string? */
      response = strdup ("dhsSysOpen() cannot connect to Supervisor");
      return(ERROR);
   }
                                                                                
fprintf (stderr, "connect to server succeeded\n");

   /* send message to supervisor asking it to start numNodes instances */
   bzero (buf, sizeof(buf));
   sprintf (buf, "%d", SYSOPEN);
   strcat (buf, ":");
   /* instances of DHS Collectors same as # of PANS - where can I find? */
   strcat (buf, "2");
   write_cnt = write (dhs_sock, buf, strlen(buf));
fprintf (stderr, "send buffer size = %d, wrote %d bytes\n",
         strlen(buf), write_cnt);
   if (write_cnt < 0) {
      perror ("dhsSysOpen() write to file failed");
      *status = errno;
      strcpy (response, "dhsSysOpen() fails to write file");
      close (dhs_sock);
      return(ERROR);
   }
   write_cnt=0;

   /* receive ACK and tear down connection */
   bzero(buf, sizeof(buf));
   if (read (dhs_sock, buf, sockbufsize) == -1) {
      perror ("dhsSysOpen() status read failed");
      *status = errno;
      strcpy (response, "dhsSysOpen() fails to read status");
      close (dhs_sock);
      return(ERROR);
   } else {     
      *status = atol (buf);
      close(dhs_sock);
      return(SUCCESS);
   }
}

