#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "../Network/network.h"


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
  int x_pos;
  int y_range[2];
  int x_colission_pos; // The X coordinates of a valid ball colission on the paddle
                       // Used on checkBallColission
} paddle;


// From main.c
extern int socketfd;
extern int nrow, ncol;
extern char mySide;
extern paddle pad;
extern ball bal;
extern double ballPositionFactor;

int gameLoop();
