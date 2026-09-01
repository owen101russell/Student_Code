/*
=========================================================
              OLED COIN COLLECTOR
=========================================================

GOAL

Turn last week's character movement project into a game!

Move your character around the screen and collect the
coin.

Each time you collect the coin:

• Your score increases.
• The coin appears somewhere else.

---------------------------------------------------------
WHAT YOU WILL LEARN

• Revising joystick movement
• Drawing another object
• Detecting collisions
• Using random()
• Keeping score

=========================================================
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ------------------------------------------------------
// Joystick Pins
// ------------------------------------------------------

const int JOY_X = A0;
const int JOY_Y = A1;

// ------------------------------------------------------
// Player
// ------------------------------------------------------

int playerX = 60;
int playerY = 30;

// ------------------------------------------------------
// Coin
// ------------------------------------------------------

int coinX = 30;
int coinY = 20;

// ------------------------------------------------------
// Score
// ------------------------------------------------------

int score = 0;

// ------------------------------------------------------
// Draw the Player
// ------------------------------------------------------

void drawPlayer()
{
  display.fillRect(playerX, playerY, 6, 6, SSD1306_WHITE);
}

// ------------------------------------------------------
// Draw the Coin
// ------------------------------------------------------

void drawCoin()
{
  display.fillCircle(coinX, coinY, 2, SSD1306_WHITE);
}

// ------------------------------------------------------
// Setup
// ------------------------------------------------------

void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();
  display.display();

  randomSeed(analogRead(A3));
}

// ------------------------------------------------------
// Main Loop
// ------------------------------------------------------

void loop()
{

  //------------------------------------------------------
  // STEP 1
  //
  // Read the joystick.
  //
  // Create TWO variables:
  //
  // xValue
  // yValue
  //
  // Hint:
  //
  // analogRead(...)
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 2
  //
  // Move the player.
  //
  // Use if statements to move:
  //
  // Left
  // Right
  // Up
  // Down
  //
  // This is revision from last lesson.
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 3
  //
  // Keep the player on the screen.
  //
  // Hint:
  //
  // constrain(...)
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 4
  //
  // Has the player collected the coin?
  //
  // The player has collected it when BOTH
  // the X position and Y position are close
  // to the coin.
  //
  // Hint:
  //
  // &&
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 5
  //
  // When the player collects the coin...
  //
  // Increase the score.
  //
  // Hint:
  //
  // score++;
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 6
  //
  // Move the coin somewhere new.
  //
  // Hint:
  //
  // random(...)
  //
  // Keep the coin on the screen.
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 7
  //
  // Clear the OLED.
  //
  // Hint:
  //
  // display.clearDisplay();
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 8
  //
  // Draw the player.
  //
  // Hint:
  //
  // drawPlayer();
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 9
  //
  // Draw the coin.
  //
  // Hint:
  //
  // drawCoin();
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 10
  //
  // Display the score.
  //
  // It should look something like:
  //
  // Score: 3
  //
  // Hints:
  //
  // display.setCursor(...)
  // display.print(...)
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 11
  //
  // Show everything on the OLED.
  //
  // Hint:
  //
  // display.display();
  //------------------------------------------------------



  delay(20);

}
