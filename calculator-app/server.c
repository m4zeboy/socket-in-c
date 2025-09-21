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
#include <string.h>

void error(const char *message)
{
  perror(message);
  exit(1);
}

int main(int argument_counter, char *argument_vector[])
{
  /* argument_counter = 2 */
  if (argument_counter < 2)
  {
    fprintf(stderr, "Port not provided. Program terminated\n");
    exit(1);
  }

  int socket_file_descriptor, new_socket_file_descriptor, port_number, result, i;
  char buffer[255];

  struct sockaddr_in server_address, client_address;

  socklen_t client_length;

  socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_file_descriptor < 0)
  {
    error("Error opening Socket.\n");
  }

  bzero((char *)&server_address, sizeof(server_address));

  port_number = atoi(argument_vector[1]);

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(port_number);

  if (bind(socket_file_descriptor, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    error("Binding failed.\n");

  listen(socket_file_descriptor, 5);
  /* 5 -> Max limit of client connections */
  client_length = sizeof(client_address);

  new_socket_file_descriptor = accept(socket_file_descriptor, (struct sockaddr *)&client_address, &client_length);

  if (new_socket_file_descriptor < 0)
    error("Error on accept.\n");

  int num1, num2, answer, choice;

S: result = write(new_socket_file_descriptor, "Enter number 1: ", strlen("Enter number 1: "));
  if(result < 0) error("Error writing to socket.\n");
  read(new_socket_file_descriptor, &num1, sizeof(int));
  printf("Client - Number 1 is: %d\n", num1);

  result = write(new_socket_file_descriptor, "Enter number 2: ", strlen("Enter number 2: "));
  if(result < 0) error("Error writing to socket.\n");
  read(new_socket_file_descriptor, &num2, sizeof(int));
  printf("Client - Number 2 is: %d\n", num2);

  result = write(new_socket_file_descriptor, "Enter your choice : \n1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n5.Exit\n",strlen("Enter your choice : \n1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n5.Exit\n"));
  if(result < 0) error("Error writing to socket.\n");
  read(new_socket_file_descriptor, &choice, sizeof(int));
  printf("Client - choice is: %d\n", choice);

  switch(choice) {
    case 1:
      answer = num1 + num2;
      break;
    case 2:
      answer = num1 - num2;
      break;
    case 3:
      answer = num1 * num2;
      break;
    case 4:
      answer = num1 / num2;
      break;
    case 5:
      goto Q;
      break;
  }

  write(new_socket_file_descriptor, &answer, sizeof(int));
  if(choice != 5) goto S;

Q: close(new_socket_file_descriptor);
  close(socket_file_descriptor);

  return 0;
}
