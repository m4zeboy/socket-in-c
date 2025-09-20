#include <stdio.h>
/*
This header file contains declarations used in most
input and output and is typically included in all C
programs

Eg: printf() and scanf() are functions of this file
*/

#include <sys/types.h>
/*
This header file contains definitions of a number
of data types used in system calls
*/

#include <sys/socket.h>
/*
This header file socket.h includes a number of definitions
and of structures needed for sockets.
Eg: Defines the sockaddr structure
*/

#include <netinet/in.h>
/*
The header file in.h contains constants and structures
needed for internet domain addresses
eg: sockaddr_in (We will be using this structure)
*/

#include <stdlib.h>
/*
The stdlib.h header defines four variable types, several
macros, and various functions for performing gerenal functions

eg: int atoi(const char *str)
Converts the string pointed to, by the argument str to an integer
(type int)
*/

#include <unistd.h>
#include <strings.h>

void error(const char *msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[])
{
  /* argc = 2 */
  if (argc < 2)
  {
    fprintf(stderr, "Port not provided. Program terminated\n");
    exit(1);
  }

  int sockfd, newsockfd, portno, n;
  char buffer[255];

  struct sockaddr_in server_addr, cli_addr;

  socklen_t clilen;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0)
  {
    error("Error opening Socket.\n");
  }

  bzero((char *)&server_addr, sizeof(server_addr));

  portno = atoi(argv[1]);

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(portno);

  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
  {
    error("Binding failed.\n");
  }
}
