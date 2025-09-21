/*
  filename server_ipaddress port_number

  argv[0] filename
  argv[1] server_ipaddress
  argv[2] port_number
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
  int socket_file_descriptor, port_number, n, i;
  struct sockaddr_in server_address;
  struct hostent *server;
  char buffer[255];

  if (argument_counter < 3)
  {
    fprintf(stderr, "usage %s hostname port\n", argument_vector[0]);
    exit(1);
  }

  port_number = atoi(argument_vector[2]);
  socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_file_descriptor < 0)
    error("Error opening socket.\n");

  server = gethostbyname(argument_vector[1]);
  if (server == NULL)
  {
    fprintf(stderr, "Error, no such host.\n");
  }

  bzero((char *)&server_address, sizeof(server_address));

  server_address.sin_family = AF_INET;
  bcopy((char *)server->h_addr_list[0], (char *)&server_address.sin_addr.s_addr, server->h_length);
  server_address.sin_port = htons(port_number);

  if (connect(socket_file_descriptor, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
  {
    error("Connection failed.\n");
  }

  int num1, num2, answer, choice;
  S: bzero(buffer, 255);
  n = read(socket_file_descriptor, buffer, 255);      /* Read server string */
  if(n < 0) error("Error reading from socket.\n");
  printf("Server - %s\n", buffer);                   
  scanf("%d", &num1);                                 /* Enter number 1*/
  write(socket_file_descriptor, &num1, sizeof(int));  /* Send number 1 to server */

  bzero(buffer, 255);
  n = read(socket_file_descriptor, buffer, 255);      /* Read server string */
  if(n < 0) error("Error reading from socket.\n");
  printf("Server - %s\n", buffer);                   
  scanf("%d", &num2);                                 /* Enter number 2*/
  write(socket_file_descriptor, &num2, sizeof(int));  /* Send number 2 to server */

  bzero(buffer, 255);
  n = read(socket_file_descriptor, buffer, 255);      /* Read server string */
  if(n < 0) error("Error reading from socket.\n");
  printf("Server - %s\n", buffer);                   
  scanf("%d", &choice);                                 /* Enter the choice */
  write(socket_file_descriptor, &choice, sizeof(int));  /* Send choic to server */

  if(choice == 5) goto Q;
  
  read(socket_file_descriptor, &answer, sizeof(int));
  printf("Server - The answer is: %d\n", answer);

  if(choice != 5) goto S;

  Q: 
  printf("You have selected to exit. Exit Successful.\n");
  close(socket_file_descriptor);
  return 0;
}