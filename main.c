#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "Game/game.h"
#include "Network/network.h"


#define IS_SERVER 2
#define IS_CLIENT 3

// Global variables
int socketfd;
int nrow, ncol;
char mySide;
paddle pad;
ball bal;

// Necessary for conversion between terminals of different sizes
char isSmallerNcols;
double ballPositionFactor;

// Debugging
void printNumberCenter(int number)
{
  printf("%d\n", number);
  getch();
}

int initializeSockets(int argc, char** argv)
{
  if(strcmp(argv[1], "-s") == 0)
	{
    // Only the socket that will be used on the comunnication is returned
    socketfd = initializeServerSocket();
    if (socketfd < 0)
    {
      printf("Error connecting\n");
      return 1;
    }
    return IS_SERVER;
  }
  else if(strcmp(argv[1], "-c") == 0)
	{
    socketfd = initializeClientSocket();
    if (socketfd <= 0)
    {
      printf("Error connecting\n");
      return 1;
    }
    return IS_CLIENT;
  }
  return 1;
}

void initializeNcursesEnvironment()
{
  // Initializes the curses environment together with its stdscr
	initscr();
  // Disable line buffering, aka don't wait for CR to process user input
  // Using cbreak() instead of raw to let controll characters enabled
	cbreak();
	// Do not print what the user presses
	noecho();
	// Enable arrow keys and function keys (F1, F2 ...)
	keypad(stdscr, TRUE);
	// Get current screen size. Not ingame resize tolerant
	getmaxyx(stdscr, nrow, ncol);
  // Timeout for the getch() function from ncurses
  timeout(0);
}

// Multiplied or divided to the current ball position so that it can be adapted
// to terminal windows of different sizes
void setBallPositionFactor(double position_factor)
{
  ballPositionFactor = position_factor;
}

void chooseSide(int return_code)
{
  char choice;
  if (return_code == IS_SERVER)
  {
    printf("\nChoose side. Options: L or R: ");
    scanf("%c", &choice);
    mySide = choice;
    sendSideChoice(choice);
  }
  // If IS_CLIENT
  else
  {
    printf("\nWaiting for server to choose side... \n");
    choice = receiveSideChoice();
    if (choice == 'L')
    {
      mySide = 'R';
    }
    else
    {
      mySide = 'L';
    }
  }
}

void endProgram()
{
	printw("Fim!!! \n");
	printw("Aperte qualquer tecla para sair");
  // The refresh() function would print the chatacters to the screen normally, but
  // the getch() ends up doing the same thing.
	refresh();
	getch();
  // Finish ncurses environment
	endwin();
}

int main(int argc, char** argv)
{
  int return_code = initializeSockets(argc, argv);
  if (return_code == 1)
  {
    closeSocket(socketfd);
    return 1;
  }

  chooseSide(return_code);
  
  closeSocket(socketfd);

  /*
	initializeNcursesEnvironment();
  // printNumberCenter(0);

  gameLoop();
	endProgram();
  */
  return 0;
}

