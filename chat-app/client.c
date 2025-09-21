/*
  filename server_ipaddress portno

  argv[0] filename
  argv[1] server_ipaddress
  argv[2] portno
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *message)
{
  perror(message);
  exit(1);
}

int main(int argument_counter, char *argument_vector[])
{
  int sockfd, portno, n, i;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char buffer[255];

  if (argument_counter < 3)
  {
    fprintf(stderr, "usage %s hostname port\n", argument_vector[0]);
    exit(1);
  }

  portno = atoi(argument_vector[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0)
    error("Error opening socket.\n");

  server = gethostbyname(argument_vector[1]);
  if (server == NULL)
  {
    fprintf(stderr, "Error, no such host.\n");
  }

  bzero((char *)&serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(portno);

  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    error("Connection failed.\n");
  }

  do
  {
    bzero(buffer, 255);
    fgets(buffer, 255, stdin);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
      error("Error on writing.\n");
    bzero(buffer, 255);
    n = read(sockfd, buffer, 255);
    if (n < 0)
      error("Error on reading.\n");

    printf("Server: %s", buffer);

    i = strncmp("Bye", buffer, 3);
  } while (i != 0);
  close(sockfd);
  return 0;
}