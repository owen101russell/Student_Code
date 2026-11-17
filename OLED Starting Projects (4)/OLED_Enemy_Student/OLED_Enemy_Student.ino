/*
=========================================================
              OLED COIN COLLECTOR
                 ENEMY CHALLENGE
=========================================================

GOAL

Collect the coin while avoiding the enemy!

The player and coin are already working.

YOUR JOB:

• Design the enemy
• Make the enemy chase the player
• Detect when the enemy catches you

---------------------------------------------------------
NEW CONCEPTS

• Simple enemy movement
• AABB collision

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

// ======================================================
// STEP 1
//
// Create the enemy.
//
// The enemy needs:
//
// • enemyX
// • enemyY
// • enemySize
//
// Give it a starting position somewhere on the screen.
//
// ======================================================


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
// Design your enemy!
// //
// Create:
//
// void drawEnemy()
//
// Your enemy should fit inside enemySize.
//
// You can use:
//
// display.fillRect()
// display.drawRect()
// display.fillCircle()
// display.drawCircle()
// display.drawLine()
// display.drawPixel()
//
// Try to make your enemy look interesting.
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
  // PLAYER MOVEMENT
  //
  // This code is already finished.
  //
  // ====================================================

  int xValue = 1023 - analogRead(JOY_X);
  int yValue = analogRead(JOY_Y);


  if (xValue < 400)
  {
    playerX -= playerSpeed;
  }

  if (xValue > 600)
  {
    playerX += playerSpeed;
  }

  if (yValue < 400)
  {
    playerY -= playerSpeed;
  }

  if (yValue > 600)
  {
    playerY += playerSpeed;
  }


  playerX = constrain(
    playerX,
    0,
    SCREEN_WIDTH - playerSize
  );

  playerY = constrain(
    playerY,
    9,
    SCREEN_HEIGHT - playerSize
  );


  // ====================================================
  // COIN
  //
  // This code is already finished.
  //
  // Remember this collision from the previous lesson.
  // ====================================================

  if (playerX < coinX + coinSize &&
      playerX + playerSize > coinX &&
      playerY < coinY + coinSize &&
      playerY + playerSize > coinY)
  {
    score++;

    coinX = random(
      0,
      SCREEN_WIDTH - coinSize
    );

    coinY = random(
      10,
      SCREEN_HEIGHT - coinSize
    );
  }


  // ====================================================
  // STEP 3
  //
  // MAKE THE ENEMY CHASE THE PLAYER
  //
  // The enemy needs to decide whether it should
  // move LEFT, RIGHT, UP or DOWN.
  //
  // Ask four questions:
  //
  // Is enemyX smaller than playerX?
  // Is enemyX bigger than playerX?
  //
  // Is enemyY smaller than playerY?
  // Is enemyY bigger than playerY?
  //
  // Hints:
  //
  // if(enemyX < playerX)
  //
  // if(enemyX > playerX)
  //
  // if(enemyY < playerY)
  //
  // if(enemyY > playerY)
  //
  // Use enemySpeed when moving.
  //
  // ====================================================



  // ====================================================
  // STEP 4
  //
  // AABB COLLISION
  //
  // How does the Arduino know the enemy
  // has caught the player?
  //
  // Imagine invisible boxes around them:
  //
  //       PLAYER
  //       ┌──────┐
  //       │      │
  //       └──────┘
  //
  //                 ENEMY
  //                 ┌──────┐
  //                 │      │
  //                 └──────┘
  //
  // The boxes collide when they overlap.
  //
  // First check HORIZONTAL overlap:
  //
  // playerX < enemyX + enemySize
  //
  // AND
  //
  // playerX + playerSize > enemyX
  //
  // Then check VERTICAL overlap:
  //
  // playerY < enemyY + enemySize
  //
  // AND
  //
  // playerY + playerSize > enemyY
  //
  // ALL FOUR conditions must be true.
  //
  // Hint:
  //
  // if(condition1 &&
  //    condition2 &&
  //    condition3 &&
  //    condition4)
  //
  // ====================================================



  // ====================================================
  // STEP 5
  //
  // WHAT HAPPENS WHEN THE ENEMY CATCHES YOU?
  //
  // Choose what you think should happen.
  //
  // A simple choice:
  //
  // • Reset the player
  // • Reset the score
  // • Move the enemy to a new position
  //
  // ====================================================



  // ====================================================
  // DRAW
  // ====================================================

  display.clearDisplay();

  drawPlayer();
  drawCoin();

  // STEP 6
  //
  // Draw your enemy.
  //
  // Hint:
  //
  // drawEnemy();
  //


  // ----------------------------------------------------
  // SCORE
  // ----------------------------------------------------

  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.print("Score: ");
  display.print(score);


  display.display();

  delay(20);

}
