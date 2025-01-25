#include "NCursesEnv.h"

// Debugging
void printNumberCenter(int number)
{
  printf("%d\n", number);
  getch();
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

void finishNcursesEnvironment()
{
  // The refresh() function would print the chatacters to the screen normally, but
  // the getch() ends up doing the same thing.
	refresh();
	getch();
  // Finish ncurses environment
	endwin();
}

