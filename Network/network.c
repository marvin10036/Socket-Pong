#include "network.h"

// Main socket of the communication
int socketfd;
int socketfd_flags;


int initializeClientSocket(char* address, char* port)
{
  // Specific struct for inet connections
  struct sockaddr_in sockfd_address;
  // Socket inet family
  sockfd_address.sin_family = AF_INET;
  // Socket inet port
  sockfd_address.sin_port = atoi(port);
  // Socket inet address field with substruct specifically for ipv4 with server's ip
  sockfd_address.sin_addr.s_addr = inet_addr(address);

  int len_sockfd_address = sizeof(sockfd_address);

  // Global variable socketfd
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  int return_code = connect(socketfd, (struct sockaddr *) &sockfd_address, (socklen_t) len_sockfd_address);
  if (return_code == -1){
    perror("connect() error");
    return 1;
  }

  return 0;
}

int initializeServerSocket(char* port)
{
  int server_sockfd;
  int len_server_address, len_client_address;

  struct sockaddr_in server_address;
  struct sockaddr_in client_address;

  server_address.sin_family = AF_INET;
  server_address.sin_port = atoi(port);
  // To use all interfaces
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);

  len_server_address = sizeof(server_address);

  int return_code;

  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  return_code = bind(server_sockfd, (struct sockaddr *) &server_address, (socklen_t) len_server_address);
  if (return_code == -1){
    perror("bind() error");
    return 1;
  }
  printf("Awaiting client connection on port %s...\n", port);
  listen(server_sockfd, 1);

  // Global variable socketfd
  socketfd = accept(server_sockfd, (struct sockaddr *) &client_address, (socklen_t *) &len_client_address);

  // Closing socket that just made the first connection
  close(server_sockfd);

  return 0;
}

void setNonBlockReading()
{
  socketfd_flags = fcntl(socketfd, F_GETFL, 0);
  fcntl(socketfd, F_SETFL, O_NONBLOCK | socketfd_flags);
}

void unsetNonBlockReading()
{
  socketfd_flags = fcntl(socketfd, F_GETFL, 0);
  fcntl(socketfd, F_SETFL, (O_NONBLOCK | socketfd_flags) ^ O_NONBLOCK);
}

void sendEndOfGameMessage()
{
  // The other side is waiting for ball parameters
  int end_game_mock_ball[3] = {-255, -255, -255};
  write(socketfd, &end_game_mock_ball, sizeof(int) * 3);
}

void sendNrow()
{
  write(socketfd, &nrow, sizeof(nrow));
}

int receiveNrow()
{
  int opponent_nrow;
  read(socketfd, &opponent_nrow, sizeof(int));
  return opponent_nrow;
}

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

void sendBall(int y_pos, int x_increment, int y_increment)
{
  int ball_info[3] = {y_pos, x_increment, y_increment};
  write(socketfd, &ball_info, sizeof(int) * 3);
}

int* receiveBall()
{
  int* ball_info = calloc(3, sizeof(int));
  read(socketfd, ball_info, sizeof(int) * 3);
  return ball_info;
}

void closeSocket()
{
  close(socketfd);
}

