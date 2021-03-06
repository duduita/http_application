/* This is the server code */
#include <bits/stdc++.h> // remember to remove it!
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "urlParser.cpp"
#include "requestParser.cpp"

#define SERVER_PORT 8080 /* arbitrary, but client & server must agree*/
#define BUF_SIZE 4096    /* block transfer size */
#define QUEUE_SIZE 10
using namespace std;

int main(int argc, char *argv[])
{
   char *host_name = argv[1];
   char *port = argv[2];
   char *dir = argv[3];
   // cout << host_name;
   // struct hostent *he;
   // he = gethostbyname(&host_name[0]);
   // in_addr ip_address = *((struct in_addr *)he->h_addr_list[0]);
   // puts(inet_ntoa(*((struct in_addr *)he->h_addr_list[0])));

   int s, b, l, fd, sa, bytes, on = 1;
   char buf[BUF_SIZE];         /* buffer for outgoing file */
   struct sockaddr_in channel; /* holds IP address */
   /* Build address structure to bind to socket. */
   memset(&channel, 0, sizeof(channel));
   /* zero channel */
   channel.sin_family = AF_INET;
   channel.sin_addr.s_addr = htons(INADDR_ANY);
   channel.sin_port = htons(SERVER_PORT);
   /* Passive open. Wait for connection. */
   s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); /* create socket */
   if (s < 0)
   {
      printf("socket call failed");
      exit(-1);
   }
   setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));

   b = bind(s, (struct sockaddr *)&channel, sizeof(channel));
   if (b < 0)
   {
      printf("bind failed");
      exit(-1);
   }

   l = listen(s, QUEUE_SIZE); /* specify queue size */
   if (l < 0)
   {
      printf("listen failed");
      exit(-1);
   }
   /* Socket is now set up and bound. Wait for connection and process it. */
   while (1)
   {
      cout << "Trying connection"
           << "\n";
      sa = accept(s, 0, 0); /* block for connection request */
      if (sa < 0)
      {
         printf("accept failed");
         exit(-1);
      }

      cout << "accept successfully"
           << "\n";

      read(sa, buf, BUF_SIZE); /* read file name from socket */
      string source = getRequestSource(buf, dir);
      cout << "dir + source: " << source << "\n";
      /* Get and return the file. */
      fd = open(&source[0], O_RDONLY); /* open the file to be sent back */
      if (fd < 0)
         printf("open failed");
      while (1)
      {
         bytes = read(fd, &source[0], BUF_SIZE); /* read from file */
         if (bytes <= 0)
            break;                     /* check for end of file */
         write(sa, &source[0], bytes); /* write bytes to socket */
      }
      close(fd); /* close file */
      close(sa); /* close connection */
   }
}
