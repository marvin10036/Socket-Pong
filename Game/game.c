#include "game.h"


void initializeGameObjects()
{
  // Set ball's standard values
	bal.x_pos = (ncol-1)/2;
	bal.y_pos = (nrow-1)/2;
  bal.x_increment = 1;
  bal.y_increment = 1;

  // Set paddle's standard values
  pad.y_range[0] = ((nrow-1)/2) - 1;
  pad.y_range[1] = ((nrow-1)/2) + 1;

  // Draw the paddle in its initial position
  mvprintw(pad.y_range[0], 2, "|\n  |\n  |");

  // Draw the ball in its initial position
  mvprintw(bal.y_pos, bal.x_pos, "0");
}

/*
 * X and Y increments
 * {x, -y}  Angle 45
 * {-x, -y} Angle 135
 * {-x, y}  Angle 225
 * {x, y}   Angle 315
*/

// Called after colission detection
void reflectAngle(char colission_type)
{
  if (colission_type == PADDLE_COLISSION){
    bal.x_increment *= (-1);
  } else{
    bal.y_increment *= (-1);
  }
}

void checkBallColission()
{
  // Paddle colission, TODO debug colission bug
  if(bal.x_pos == 3 && bal.y_pos >= pad.y_range[0] && bal.y_pos <= pad.y_range[1])
  {
    reflectAngle(PADDLE_COLISSION);
  }
  // Horizontal wall colission
  else if(bal.y_pos == 0 || bal.y_pos == nrow - 1)
  {
    reflectAngle(WALL_COLISSION);
  }
  // Vertical wall colission
  else if(bal.x_pos == 0 || bal.x_pos == ncol - 1)
  {
    // TODO remover e colocar condição de fim de jogo
    reflectAngle(PADDLE_COLISSION);
  }
}

void updateBallPos(int new_x_pos, int new_y_pos)
{
	bal.x_pos = new_x_pos;
	bal.y_pos = new_y_pos;
}

void updatePaddlePos(int new_y0_pos, int new_y1_pos)
{
	pad.y_range[0] = new_y0_pos;
	pad.y_range[1] = new_y1_pos;
}

void moveBall()
{
  mvprintw(bal.y_pos, bal.x_pos, " ");
  mvprintw(bal.y_pos + bal.y_increment, bal.x_pos + bal.x_increment, "0");
  updateBallPos(bal.x_pos + bal.x_increment, bal.y_pos + bal.y_increment);
}

void movePaddle(int input_key)
{
  switch(input_key){
    case KEY_UP:
      // If it isn't on the screen upper limit
      if (pad.y_range[0] > 0){
        mvprintw(pad.y_range[1], 2, " ");
        mvprintw((pad.y_range[0] - 1), 2, "|");
        updatePaddlePos(pad.y_range[0] - 1, pad.y_range[1] - 1);
      }
      break;
    case KEY_DOWN:
      // If it isn't on the screen lower limit
      if (pad.y_range[1] < nrow - 1){
        mvprintw(pad.y_range[0], 2, " ");
        mvprintw((pad.y_range[1] + 1), 2, "|");
        updatePaddlePos(pad.y_range[0] + 1, pad.y_range[1] + 1);
      }
      break;
    default:
      break;
  }
}

void gameLoop()
{
  initializeGameObjects();

  // Game loop
  int input_key;
  long iter_counter = 0;
  while(1){
    input_key = getch();
    movePaddle(input_key);
    // TODO Encontrar maneira melhor de fazer game clock tick
    if (iter_counter == 200000){
      moveBall();
      checkBallColission();
      iter_counter = 0;
    }
    iter_counter++;
  }
}

