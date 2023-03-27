#include "Snake.h"

// Updates snakes velocity in x and y planes according to snake's direction
void Snake::updateVelocity()
{
  switch (direction)
  {
  case STOP:
    yVelocity = 0;
    xVelocity = 0;
    break;
  case UP:
    if (yVelocity == -1) // Can't go up if going down, same for left and right
    {
      break;
    }

    yVelocity = 1;
    xVelocity = 0;
    break;
  case DOWN:
    if (yVelocity == 1)
    {
      break;
    }

    yVelocity = -1;
    xVelocity = 0;
    break;
  case LEFT:
    if (xVelocity == -1)
    {
      break;
    }

    yVelocity = 0;
    xVelocity = 1;
    break;
  case RIGHT:
    if (xVelocity == 1)
    {
      break;
    }

    yVelocity = 0;
    xVelocity = -1;
    break;
  }
}

// Move all snake parts one unit in the direction snake is heading
void Snake::updatePosition()
{
  for (int i = tailLength - 1; i > 0; i--)
  {
    tailParts[i] = tailParts[i - 1];
  }

  tailParts[0].x = head.x;
  tailParts[0].y = head.y;

  head.x = head.x + xVelocity;
  head.y = head.y + yVelocity;
}

// Check that snake's head is within game area
bool Snake::isOutOfBounds()
{
  return (head.x > 7 || head.x < 0 || head.y > 7 || head.y < 0);
}