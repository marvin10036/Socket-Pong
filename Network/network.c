#include "network.h"

int initializeClientSocket()
{
  // Specific struct for inet connections
  struct sockaddr_in sockfd_address;
  // Socket inet family
  sockfd_address.sin_family = AF_INET;
  // Socket inet port
  sockfd_address.sin_port = 9734;
  // Socket inet address field with substruct specifically for ipv4 with server's ip
  sockfd_address.sin_addr.s_addr = inet_addr("192.168.0.106");

  int len_sockfd_address = sizeof(sockfd_address);

  // Global variable socketfd
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  int return_code = connect(socketfd, (struct sockaddr *) &sockfd_address, (socklen_t) len_sockfd_address);
  if (return_code == -1){
    perror("connect() error");
    return 1;
  }
  char ch[2] = "OK";
  write(socketfd, &ch, 2);
  return 0;
}

int initializeServerSocket()
{
  int server_sockfd;
  int len_server_address, len_client_address;

  struct sockaddr_in server_address;
  struct sockaddr_in client_address;

  server_address.sin_family = AF_INET;
  server_address.sin_port = 9734;
  // Para usar todas as interfaces
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);

  len_server_address = sizeof(server_address);

  char ch[2];
  int return_code;

  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  return_code = bind(server_sockfd, (struct sockaddr *) &server_address, (socklen_t) len_server_address);
  if (return_code == -1){
    perror("bind() error");
    return 1;
  }
  printf("Awaiting client connection...\n");
  listen(server_sockfd, 1);

  // Global variable socketfd
  socketfd = accept(server_sockfd, (struct sockaddr *) &client_address, (socklen_t *) &len_client_address);
  read(socketfd, &ch, 2);
  printf("%s\n", ch);

  // Closing socket that just made the first connection
  closeSocket(server_sockfd);
  return 0;
}

void sendNcols()
{
  write(socketfd, &ncol, sizeof(ncol));
}

/*
int readNcols()
{
  int opponent_ncols;
  read(socketfd, ncols, sizeof(ncols));
}
*/

void sendSideChoice(char choice)
{
  write(socketfd, &choice, 1);
}

char receiveSideChoice()
{
  char choice;
  read(socketfd, &choice, 1);
  return choice;
}

void closeSocket(int sockfd)
{
  close(sockfd);
}

