#ifndef SNAKE_H
#define SNAKE_H

#include "Point.h"

const int MAX_LENGTH = 63; // 63 for 8x8 screen + head

enum Direction
{
  STOP = 0,
  UP,
  DOWN,
  LEFT,
  RIGHT
};

class Snake
{
public:
  Snake() = default;
  ~Snake() = default;

  void updateVelocity();

  void updatePosition();

  bool isOutOfBounds();

  Point head;
  Point tailParts[MAX_LENGTH];
  int tailLength;
  int yVelocity;
  int xVelocity;
  Direction direction;
};

#endif