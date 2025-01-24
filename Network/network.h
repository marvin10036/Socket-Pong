#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

// From main.c
extern int ncol, nrow;
extern char mySide;


int initializeClientSocket(char* address, char* port);

int initializeServerSocket(char* port);

void setNonBlockReading();

void unsetNonBlockReading();

void sendEndOfGameMessage();

void sendNrow();

int receiveNrow();

void sendSideChoice(char choice);

char receiveSideChoice();

void sendBall(int y_pos, int x_increment, int y_increment);

int* receiveBall();

void closeSocket();

