/*
=========================================================
              OLED COIN COLLECTOR
                 ENEMY CHALLENGE
=========================================================

GOAL

Collect coins while avoiding an enemy!

First, rebuild the coin collector you made before.

Then create your own enemy and make it chase you.

---------------------------------------------------------
REVISION

• Joystick movement
• if statements
• Variables
• random()
• OLED drawing
• AABB collision

---------------------------------------------------------
NEW

• Enemy movement
• Enemy collision

=========================================================
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// ------------------------------------------------------
// Joystick
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

int score = 0;


// ------------------------------------------------------
// Enemy
// ------------------------------------------------------

// STEP 1
//
// Create the enemy's:
// • X position
// • Y position
// • Size
//
// Give the enemy a starting position somewhere
// on the screen.
//
// Hints:
//
// enemyX
// enemyY
// enemySize
//
// ------------------------------------------------------


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
  display.fillCircle(
    coinX,
    coinY,
    2,
    SSD1306_WHITE
  );
}


// ======================================================
// DRAW ENEMY
// ======================================================

// STEP 2
//
// Design your enemy!
//
// Create:
//
// void drawEnemy()
//
// Use the OLED drawing functions you already know.
//
// Try to make it look different from the player.
//
// Keep it inside the enemySize box.
//
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
  // STEP 3 — READ THE JOYSTICK
  // ====================================================
  //
  // Create:
  //
  // xValue
  // yValue
  //
  // Remember:
  // The X axis is flipped.
  //
  // Hint:
  //
  // 1023 - analogRead(...)
  //
  // ====================================================



  // ====================================================
  // STEP 4 — MOVE THE PLAYER
  // ====================================================
  //
  // Revision!
  //
  // Move:
  // • Left
  // • Right
  // • Up
  // • Down
  //
  // Use playerSpeed.
  //
  // ====================================================



  // ====================================================
  // STEP 5 — KEEP THE PLAYER ON SCREEN
  // ====================================================
  //
  // Hint:
  //
  // constrain(...)
  //
  // ====================================================



  // ====================================================
  // STEP 6 — COLLECT THE COIN
  // ====================================================
  //
  // Use AABB collision.
  //
  // The coin is a small box.
  //
  // When the player touches the coin:
  //
  // • Increase score
  // • Give the coin a new random position
  //
  // Remember:
  //
  // Two boxes must overlap:
  //
  // horizontally AND vertically.
  //
  // ====================================================



  // ====================================================
  // STEP 7 — MAKE THE ENEMY CHASE
  // ====================================================
  //
  // The enemy needs to move towards the player.
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
  // Use enemySpeed.
  //
  // ====================================================



  // ====================================================
  // STEP 8 — AABB ENEMY COLLISION
  // ====================================================
  //
  // Now use AABB again!
  //
  // This time compare:
  //
  // PLAYER
  // +
  // ENEMY
  //
  // All FOUR checks must be true.
  //
  // Horizontal:
  //
  // playerX < enemyX + enemySize
  //
  // playerX + playerSize > enemyX
  //
  // Vertical:
  //
  // playerY < enemyY + enemySize
  //
  // playerY + playerSize > enemyY
  //
  // If all four are true:
  //
  // THE ENEMY CAUGHT YOU!
  //
  // ====================================================



  // ====================================================
  // STEP 9 — WHAT HAPPENS WHEN YOU GET CAUGHT?
  // ====================================================
  //
  // Decide what happens.
  //
  // For example:
  //
  // • Reset the score
  // • Return the player to the starting position
  // • Move the enemy somewhere else
  //
  // ====================================================



  // ====================================================
  // STEP 10 — DRAW THE GAME
  // ====================================================
  //
  // Clear the screen.
  //
  // Draw:
  //
  // • Player
  // • Coin
  // • Enemy
  //
  // Display the score.
  //
  // Finally:
  //
  // display.display();
  //
  // ====================================================



  delay(20);

}
