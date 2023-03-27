#include <avr/wdt.h> // Library for watchdog timer
#include "Game.h"

// Initialize matrix values
void Game::setup()
{
    matrix.begin(0x70);
    matrix.setBrightness(5);
}

// Initialize game starting point
void Game::init()
{
    gameOver = false;

    // Reset all parts of snake
    for (auto &part : snake.tailParts)
    {
        part.x = 0;
        part.y = 0;
    }

    snake.tailLength = 1;
    snake.direction = STOP;
    // Place snake's head roughly in the middle
    snake.head.x = matrix.width() / 2;
    snake.head.y = matrix.height() / 2;

    // Update position to render tail immediately
    snake.updatePosition();

    // Set seed for rng and set new fruit position
    randomSeed(analogRead(A0));
    setFruitPosition();
}

// Executes every timer interrupt
void Game::tick()
{
    handleFruitEating();
    snake.updateVelocity();
    snake.updatePosition();

    if (snake.direction != STOP)
    {
        gameOver = isCollidingWithTail(snake.head) || snake.isOutOfBounds();
    }
}

// Draw snake and fruit on screen
void Game::draw()
{
    matrix.clear();
    // Draw snake
    matrix.drawPixel(snake.head.x, snake.head.y, LED_ON);

    for (int i = snake.tailLength - 1; i >= 0; i--)
    {
        matrix.drawPixel(snake.tailParts[i].x, snake.tailParts[i].y, LED_ON);
    }

    // Don't draw fruit if game is over (for game over animation)
    if (!gameOver)
    {
        matrix.drawPixel(fruit.x, fruit.y, LED_ON);
    }

    matrix.writeDisplay();
}

// Get key press event from keypad and change snake's direction accordingly
void Game::handleKeyPress(KeypadEvent key)
{
    switch (key)
    {
    case '8':
        snake.direction = UP;
        break;
    case '0':
        snake.direction = DOWN;
        break;
    case '*':
        snake.direction = LEFT;
        break;
    case '#':
        snake.direction = RIGHT;
        break;
    case 'A': // Press the 'A' key to restart game
        init();
        break;
    }
}

// Set fruit to new position
void Game::setFruitPosition()
{
    bool validPosition{false};
    Point newFood;

    while (!validPosition)
    {
        newFood = generateRandomPoint();

        // If the food is not on top of the snake, is a valid position
        if (!isCollidingWithHead(newFood) && !isCollidingWithTail(newFood))
        {
            validPosition = true;
        }
    }

    // Assign new values to fruit variable
    fruit.x = newFood.x;
    fruit.y = newFood.y;
}

// Grow snake and set new fruit position if snake eats fruit
void Game::handleFruitEating()
{
    if (isCollidingWithHead(fruit))
    {
        snake.tailLength++;
        setFruitPosition();
    }
}

// Animate game over screen and initialize new game
void Game::handleGameOver()
{
    // Disable timer and watchdog so animation doesn't get interrupted
    TIMSK1 = 0;
    wdt_disable();

    snake.direction = STOP;

    // Flash snake 3 times
    for (int i = 0; i < 3; i++)
    {
        matrix.clear();
        matrix.writeDisplay();
        delay(500);
        draw();
        delay(500);
    }

    // "Remove" snake parts one by one
    for (int i = snake.tailLength - 1; i >= 0; i--)
    {
        matrix.drawPixel(snake.tailParts[i].x, snake.tailParts[i].y, LED_OFF);
        matrix.writeDisplay();
        delay(200);
    }

    matrix.drawPixel(snake.head.x, snake.head.y, LED_OFF);
    matrix.writeDisplay();

    delay(1000);
    // Initialize a new game
    init();

    // Re-enable timer and watchdog
    TIMSK1 |= (1 << OCIE1A);
    wdt_enable(WDTO_2S);
}

// Return the value of gameOver
bool Game::getGameOver()
{
    return gameOver;
}

// Check if point is colliding with snake's head
bool Game::isCollidingWithHead(Point point)
{
    return (point.x == snake.head.x && point.y == snake.head.y);
}

// Loop through snake's tail and check if point is colliding with it
bool Game::isCollidingWithTail(Point point)
{
    for (int i = 0; i < snake.tailLength; i++)
    {
        if (point.x == snake.tailParts[i].x && point.y == snake.tailParts[i].y)
        {
            return true;
        }
    }

    return false;
}

// Returns a Point struct with random values from 0 to 8
Point Game::generateRandomPoint()
{
    return {(int)random(8), (int)random(8)};
}
