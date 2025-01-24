#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "Game/game.h"
#include "Network/network.h"
#include "NCursesEnv/NCursesEnv.h"


#define INPUT_ERROR 1
#define IS_SERVER 2
#define IS_CLIENT 3

// Global variables
int nrow, ncol;
char mySide; // L or R
paddle pad;
ball bal;

// Necessary for conversion between terminals of different sizes
double ballPositionFactor;


// If it's server or client
int getRole(int argc, char** argv)
{
  if(argc == 4 && strcmp(argv[1], "-s") == 0 && strcmp(argv[2], "-p") == 0)
  {
    return IS_SERVER;
  }
  else if(argc == 6 && strcmp(argv[1], "-c") == 0 && strcmp(argv[2], "-h") == 0 && strcmp(argv[4], "-p") == 0)
	{
    return IS_CLIENT;
  }
  return INPUT_ERROR;
}


int initializeSockets(int role, char** argv)
{
  int socket_status;

  if(role == IS_SERVER)
  {
    // Only the socket that will be used on the communication is returned
    socket_status = initializeServerSocket(argv[3]);
    if (socket_status != 0)
    {
      printf("Error connecting\n");
      return 1;
    }
  }
  else if(role == IS_CLIENT)
	{
    socket_status = initializeClientSocket(argv[3], argv[5]);
    if (socket_status != 0)
    {
      printf("Error connecting\n");
      return 1;
    }
  }
  return 0;
}

void chooseSide(int role)
{
  if (role == IS_SERVER)
  {
    printf("\nChoose side. Options: L or R: ");
    char choice = getchar();
    if (choice != 'L' && choice != 'R')
    {
      choice = 'L';
    }

    mySide = choice;
    sendSideChoice(choice);

    printf("\nYour side is: %c\n", mySide);
  }
  // If IS_CLIENT
  else
  {
    printf("\nWaiting for server to choose side... \n");

    char choice = receiveSideChoice();
    if (choice == 'L')
    {
      mySide = 'R';
    }
    else
    {
      mySide = 'L';
    }
    printf("Your side is: %c\n", mySide);
  }
}

// The ballPositionFactor  is multiplied to the current ball position so that
// it can be adapted to terminal windows of different sizes
void setBallPositionFactor()
{
  // Y axis exchange
  sendNrow();
  int opponent_nrow = receiveNrow();

  if(nrow < opponent_nrow)
  {
    ballPositionFactor = 1 / ((double) opponent_nrow / nrow);
  }
  else
	{
    ballPositionFactor = (double) nrow / opponent_nrow;
  }
}

int main(int argc, char** argv)
{
  int role = getRole(argc, argv);
  if (role == INPUT_ERROR)
  {
    printf("Input error\n");
    return 1;
  }

  int status_code = initializeSockets(role, argv);
  if (status_code == 1)
  {
    printf("\nIt's possible that the port is on TIME_WAIT, change the port or wait untill it's released.\n");
    return 1;
  }

  chooseSide(role);

  // To get nrow value before setBallPositionFactor()
	initializeNcursesEnvironment();

  // Involves Server and Client Y axis exchange
  setBallPositionFactor();

  int won = gameLoop();
	finishNcursesEnvironment();

  if (won)
  {
    printf("You won.\n");
  }
  else
  {
    printf("You lost.\n");
  }

  closeSocket();
  return 0;
}

