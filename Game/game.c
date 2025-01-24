#include "game.h"

// Gonna be returned on checkBallColission() when it hits the opponent's wall
#define BALL_SENT 1
// Same thing, but when it hits your vertical wall
#define LOST 2


// For receiveBall();
void unmarshallAndUpdateBall(int* marshalledBall)
{
  if (mySide == 'L')
  {
    bal.x_pos = ncol - 1;
  }
  else
  {
    bal.x_pos = 0;
  }

  bal.y_pos = marshalledBall[0] * ballPositionFactor;
  bal.x_increment = marshalledBall[1];
  bal.y_increment = marshalledBall[2];
  free(marshalledBall);
}

void initializeBall()
{
  // Set ball's standard values
	bal.x_pos = (ncol-1)/2;
	bal.y_pos = (nrow-1)/2;
  bal.x_increment = -1;
  bal.y_increment = 1;

  // Draw the ball in its initial position
  mvprintw(bal.y_pos, bal.x_pos, "0");
}

void initializePaddle()
{
  // Set paddle's Y standard values
  pad.y_range[0] = ((nrow-1)/2) - 1;
  pad.y_range[1] = ((nrow-1)/2) + 1;

  if (mySide == 'L')
  {
    pad.x_pos = 2;
    pad.x_colission_pos = pad.x_pos + 1;
  }
  else
  {
    pad.x_pos = ncol - 3;
    pad.x_colission_pos = pad.x_pos - 1;
  }

  // Draw the paddle in its initial position
  mvprintw(pad.y_range[0], pad.x_pos, "|");
  mvprintw(pad.y_range[0] + 1, pad.x_pos, "|");
  mvprintw(pad.y_range[0] + 2, pad.x_pos, "|");
}

/*
 * X and Y increments
 * {x, -y}  Angle 45
 * {-x, -y} Angle 135
 * {-x, y}  Angle 225
 * {x, y}   Angle 315
*/

// Called after colission detection on checkBallColission()
void reflectAngle(char colission_type)
{
  if (colission_type == PADDLE_COLISSION){
    bal.x_increment *= (-1);
  }
  else
  {
    bal.y_increment *= (-1);
  }
}

int checkBallColission()
{
  // Paddle colission, TODO debug colission bug
  if(bal.x_pos == pad.x_colission_pos && bal.y_pos >= pad.y_range[0] && bal.y_pos <= pad.y_range[1])
  {
    reflectAngle(PADDLE_COLISSION);
  }
  // Horizontal wall colission
  else if(bal.y_pos == 0 || bal.y_pos == nrow - 1)
  {
    reflectAngle(WALL_COLISSION);
  }
  // Left Vertical wall colission
  else if(bal.x_pos == 0)
  {
    if (mySide == 'L')
    {
      return LOST;
    }
    else
    {
      // Eliminating ball's last position before sending it
      mvprintw(bal.y_pos, bal.x_pos, " ");
      sendBall(bal.y_pos, bal.x_increment, bal.y_increment);
      return BALL_SENT;
    }
  }
  // Right Vertical wall colission
  else if(bal.x_pos == ncol - 1)
  {
    if (mySide == 'R')
    {
      return LOST;
    }
    else
    {
      // Eliminating ball's last position before sending it
      mvprintw(bal.y_pos, bal.x_pos, " ");
      sendBall(bal.y_pos, bal.x_increment, bal.y_increment);
      return BALL_SENT;
    }
  }
  return 0;
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
        mvprintw(pad.y_range[1], pad.x_pos, " ");
        mvprintw((pad.y_range[0] - 1), pad.x_pos, "|");
        updatePaddlePos(pad.y_range[0] - 1, pad.y_range[1] - 1);
      }
      break;
    case KEY_DOWN:
      // If it isn't on the screen lower limit
      if (pad.y_range[1] < nrow - 1){
        mvprintw(pad.y_range[0], pad.x_pos, " ");
        mvprintw((pad.y_range[1] + 1), pad.x_pos, "|");
        updatePaddlePos(pad.y_range[0] + 1, pad.y_range[1] + 1);
      }
      break;
    default:
      break;
  }
}

// Try to read the socket with receiveBall()
char waitForBall(char waiting_for_ball)
{
  int* marshalledBall = receiveBall();

  // End of game condition
  if (marshalledBall[1] == -255)
  {
    return -1;
  }
  // As the reads are now non-blocking, it will read a 0 if there's nothing
  // there, and something different if there is something there
  if (marshalledBall[1] != 0)
  {
    unmarshallAndUpdateBall(marshalledBall);
    waiting_for_ball = 0;
  }

  return waiting_for_ball;
}

char initializeGameObjects()
{
  initializePaddle();

  char waiting_for_ball = 1;
  if (mySide == 'L')
  {
    waiting_for_ball = 0;
    initializeBall();
  }

  return waiting_for_ball;
}

int gameLoop()
{
  // Boolean condition
  char waiting_for_ball = initializeGameObjects();

  // Reading from Sockets are gonna be non-blocking while the game runs
  // So that the player may move the paddle while it waits for the opponent
  setNonBlockReading();

  // Game loop
  int input_key, colission_status;
  long game_clock = 0;

  while(1){
    input_key = getch();
    movePaddle(input_key);

    if (game_clock == 145000){
      // If you are waiting for the oponnent to pass the ball
      if (waiting_for_ball)
      {
        // It is essentially busy waiting, could be a thread for better performance
        waiting_for_ball = waitForBall(waiting_for_ball);
        // End of game, you won
        if (waiting_for_ball == -1)
        {
          return 1;
        }
      }
      // If the ball is on your side
      else
      {
        moveBall();
        colission_status = checkBallColission();
        if (colission_status == BALL_SENT)
        {
          waiting_for_ball = 1;
        }
        else if (colission_status == LOST)
        {
          // End of the game, you lost
          sendEndOfGameMessage();
          return 0;
        }
      }
      game_clock = 0;
    }
    game_clock++;
  }

  // Making reading from Sockets blocking again
  unsetNonBlockReading();
}

