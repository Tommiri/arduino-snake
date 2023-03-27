#include "Arduino.h"              // Required for Arduino programs built outside Arduino IDE
#include <avr/wdt.h>              // Library for watchdog timer
#include <Keypad.h>               // Library for keypad functionality
#include <Adafruit_GFX.h>         // Library for drawing graphics on Adafruit screens
#include "Adafruit_LEDBackpack.h" // Library for supporting LED-matrix screens

#include "Game.h"

// Initialize keypad
const byte ROWS{4};
const byte COLS{4};

char hexaKeys[ROWS][COLS]{
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

byte colPins[COLS]{5, 4, 3, 2};
byte rowPins[ROWS]{9, 8, 7, 6};

Keypad keypad{Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS)};
void handleKeypadEvent(KeypadEvent key);

// Initialize LED matrix
Adafruit_8x8matrix matrix{Adafruit_8x8matrix()};

// Initialize timer values
const unsigned long CPU_FREQ{16000000};                // ATmega328 clock speed, 16 MHz
float updateFrequency{2.5};                            // Update frequency in Hz
double interval{(CPU_FREQ / 256.0) / updateFrequency}; // Calculate interrupt interval (256 for prescaler)

Game game(keypad, matrix);

void setup()
{
    Serial.begin(9600);
    wdt_enable(WDTO_2S); // Enable 2 second watchdog timer

    // Configure timer
    noInterrupts();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = interval; // Set compare value to desired interrupt interval
    // Set prescaler of 256 and enable compare match interrupt
    TCCR1B |= (1 << CS12) | (1 << WGM12);
    TIMSK1 |= (1 << OCIE1A);
    interrupts();

    // Setup and initialize game
    keypad.addEventListener(handleKeypadEvent);
    game.setup();
    game.init();
}

ISR(TIMER1_COMPA_vect)
{
    game.tick(); // Update game every interrupt
}

void loop()
{
    // While game is running
    while (!game.getGameOver())
    {
        game.draw();     // Keep redrawing game
        keypad.getKey(); // Check for keypad eve*nt to trigger event listener
        wdt_reset();     // Reset watchdog timer at end of every loop
    }

    game.handleGameOver();
}

void handleKeypadEvent(KeypadEvent key)
{
    // If a key is pressed, handle it
    if (keypad.getState() == PRESSED)
    {
        game.handleKeyPress(key);
    }
}