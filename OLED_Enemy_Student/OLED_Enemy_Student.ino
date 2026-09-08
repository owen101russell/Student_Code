/*
=========================================================
              OLED COIN COLLECTOR
                 ENEMY EDITION
=========================================================

GOAL

Collect as many coins as possible while avoiding
the enemy!

The enemy slowly follows the player.

---------------------------------------------------------
WHAT YOU WILL PRACTISE

• Variables
• if statements
• Joystick movement
• OLED drawing
• AABB collision
• Random positions

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

const int JOY_X = A1;
const int JOY_Y = A0;


// ------------------------------------------------------
// Game Settings
// ------------------------------------------------------

const int playerSpeed = 2;
const int enemySpeed = 1;


// ------------------------------------------------------
// Player
// ------------------------------------------------------

int playerX = 60;
int playerY = 30;

const int playerSize = 6;


// ------------------------------------------------------
// Coin
// ------------------------------------------------------

int coinX = 30;
int coinY = 20;

const int coinSize = 4;


// ------------------------------------------------------
// Enemy
// ------------------------------------------------------

// STEP 1
//
// Create the enemy's X and Y position,
// and its size.
//
// Hints:
//
// enemyX
// enemyY
// enemySize
//
// Give it a starting position on the screen.
// ------------------------------------------------------


// ------------------------------------------------------
// Score
// ------------------------------------------------------

int score = 0;


// ======================================================
// DRAW PLAYER
// ======================================================

void drawPlayer()
{
  display.fillRect(
    playerX,
    playerY,
    playerSize,
    playerSize,
    SSD1306_WHITE
  );
}


// ======================================================
// DRAW COIN
// ======================================================

void drawCoin()
{
  display.drawCircle(
    coinX + 2,
    coinY + 2,
    2,
    SSD1306_WHITE
  );

  display.drawPixel(
    coinX + 2,
    coinY + 2,
    SSD1306_WHITE
  );
}


// ======================================================
// DRAW ENEMY
// ======================================================

// STEP 2
//
// Create a function called drawEnemy().
//
// Draw the enemy as a simple rectangle.
//
// Hint:
//
// display.fillRect(...)
//
// Use the enemy's:
// • X
// • Y
// • Size
// ======================================================


// ======================================================
// SETUP
// ======================================================

void setup()
{
  display.begin(
    SSD1306_SWITCHCAPVCC,
    0x3C
  );

  display.clearDisplay();
  display.display();

  randomSeed(analogRead(A3));
}


// ======================================================
// MAIN LOOP
// ======================================================

void loop()
{

  // ====================================================
  // STEP 3
  //
  // READ THE JOYSTICK
  //
  // Create xValue and yValue.
  //
  // Remember:
  // The X axis needs to be flipped.
  //
  // Hint:
  //
  // 1023 - analogRead(...)
  // ====================================================



  // ====================================================
  // STEP 4
  //
  // MOVE THE PLAYER
  //
  // This is revision.
  //
  // Move:
  // • Left
  // • Right
  // • Up
  // • Down
  //
  // Use playerSpeed.
  // ====================================================



  // ====================================================
  // STEP 5
  //
  // KEEP THE PLAYER ON SCREEN
  //
  // Hint:
  //
  // constrain(...)
  // ====================================================



  // ====================================================
  // STEP 6
  //
  // COLLECT THE COIN
  //
  // Use the AABB collision code from
  // the previous lesson.
  //
  // When the player touches the coin:
  //
  // • score increases
  // • coin gets a new random position
  // ====================================================



  // ====================================================
  // STEP 7
  //
  // MOVE THE ENEMY
  //
  // Make the enemy follow the player.
  //
  // Ask:
  //
  // Is enemyX less than playerX?
  //
  // Is enemyX greater than playerX?
  //
  // Then do the same thing for Y.
  //
  // Hints:
  //
  // if(enemyX < playerX)
  // if(enemyX > playerX)
  //
  // if(enemyY < playerY)
  // if(enemyY > playerY)
  //
  // Move using enemySpeed.
  // ====================================================



  // ====================================================
  // STEP 8
  //
  // CHECK FOR ENEMY COLLISION
  //
  // Use AABB again.
  //
  // This time, compare:
  //
  // Player Box
  // +
  // Enemy Box
  //
  // What should happen when they overlap?
  // ====================================================



  // ====================================================
  // STEP 9
  //
  // RESET THE GAME AFTER BEING CAUGHT
  //
  // When the enemy catches the player:
  //
  // • Reset the score
  // • Reset the player position
  // • Move the enemy somewhere else
  //
  // Use random() for the enemy's new position.
  // ====================================================



  // ====================================================
  // STEP 10
  //
  // DRAW EVERYTHING
  //
  // Clear the screen.
  //
  // Draw:
  // • Player
  // • Coin
  // • Enemy
  //
  // Display the score.
  //
  // Finally:
  //
  // display.display();
  // ====================================================



  delay(20);

}
