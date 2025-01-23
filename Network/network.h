#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int initializeClientSocket();

int initializeServerSocket();

void sendNcols();

void sendSideChoice(char choice);

char receiveSideChoice();

void closeSocket();

// From main.c
extern int ncol, nrow;
