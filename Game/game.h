#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>


#define PADDLE_COLISSION 0
#define WALL_COLISSION 1

typedef struct
{
	int x_pos;
	int y_pos;
  int x_increment;
  int y_increment;
} ball;

typedef struct
{
  int y_range[2];
} paddle;


// From the main.c
extern int socketfd;
extern int nrow, ncol;
extern paddle pad;
extern ball bal;

void gameLoop();
