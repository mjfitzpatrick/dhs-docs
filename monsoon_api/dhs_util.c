#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
                                                        

/* utility function to declare a socket on this machine and connect to  */
/* the one specified as arguments. */

int
connectTo (char *machine, int port)
{
   int my_sock;
   struct sockaddr_in host_addr;
   struct hostent *hp;

   /* socket setup */
   my_sock = socket (AF_INET, SOCK_STREAM, 0);
   if (my_sock == -1) {
      perror ("open socket error");
      return(-1);
   }

   hp = gethostbyname (machine);
   if (hp == NULL) {
      perror("gethostbyname error");
      close(my_sock);
      return(-1);
   }

   bcopy(hp->h_addr_list[0],(char *)&host_addr.sin_addr.s_addr,hp->h_length);
   host_addr.sin_family = AF_INET;
   host_addr.sin_port = htons(port);

fprintf (stderr, "socket = %d host_addr setup\n", my_sock);

   /* connect */
   if (connect (my_sock, (struct sockaddr *)&host_addr, sizeof(host_addr)) == -1) {
      perror("connect failed");
      close(my_sock);
      return(-1);
   }

fprintf (stderr, "connect to server succeeded\n");
return(my_sock);
}
