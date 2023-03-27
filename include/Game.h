#ifndef GAME_H
#define GAME_H

#include <Keypad.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#include "Snake.h"
#include "Point.h"

class Game
{
public:
  Game(const Keypad &aKeypad, const Adafruit_8x8matrix &aMatrix) : matrix(aMatrix), keypad(aKeypad){};
  ~Game() = default;

  void setup();

  void init();

  void tick();

  void draw();

  void handleKeyPress(KeypadEvent key);

  void handleGameOver();

  bool getGameOver();

private:
  Adafruit_8x8matrix matrix;
  Keypad keypad;
  Snake snake{};
  Point fruit{};
  bool gameOver{};

  void setFruitPosition();

  void handleFruitEating();

  bool isCollidingWithHead(Point point);

  bool isCollidingWithTail(Point point);

  Point generateRandomPoint();
};

#endif