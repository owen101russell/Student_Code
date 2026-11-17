/*
=========================================================
              OLED COIN COLLECTOR
=========================================================

GOAL

Move your character around the OLED and collect
as many coins as possible.

---------------------------------------------------------
YOU WILL PRACTISE

• Joystick movement
• AABB collision
• random()
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
// Joystick
// ------------------------------------------------------

const int JOY_X = A1;
const int JOY_Y = A0;


// ------------------------------------------------------
// Player
// ------------------------------------------------------

int playerX = 60;
int playerY = 30;

const int playerSize = 6;
const int playerSpeed = 2;


// ------------------------------------------------------
// Coin
// ------------------------------------------------------

int coinX = 30;
int coinY = 20;

const int coinSize = 4;

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
  display.fillCircle(
    coinX,
    coinY,
    2,
    SSD1306_WHITE
  );
}


// ======================================================
// SETUP
// ======================================================

void setup()
{
  display.begin(
    SSD1306_SWITCHCAPVCC,
    0x3C
  );

  randomSeed(analogRead(A3));
}


// ======================================================
// MAIN LOOP
// ======================================================

void loop()
{

  // ====================================================
  // STEP 1
  //
  // READ THE JOYSTICK
  //
  // Create:
  //
  // xValue
  // yValue
  //
  // The X axis is flipped because of the joystick's
  // physical orientation.
  //
  // Hint:
  //
  // 1023 - analogRead(...)
  // ====================================================



  // ====================================================
  // STEP 2
  //
  // MOVE THE PLAYER
  //
  // Move left, right, up and down.
  //
  // Use:
  //
  // if()
  // playerX
  // playerY
  // playerSpeed
  // ====================================================



  // ====================================================
  // STEP 3
  //
  // KEEP THE PLAYER ON THE SCREEN
  //
  // Hint:
  //
  // constrain(...)
  // ====================================================



  // ====================================================
  // STEP 4
  //
  // CHECK FOR A COIN COLLISION
  //
  // Use AABB.
  //
  // The player and coin are both boxes.
  //
  // Remember:
  //
  // They must overlap horizontally
  // AND
  // vertically.
  //
  // ====================================================



  // ====================================================
  // STEP 5
  //
  // WHEN THE PLAYER COLLECTS THE COIN:
  //
  // • Increase the score
  // • Move the coin to a random position
  //
  // Hints:
  //
  // score++;
  // random(...)
  // ====================================================



  // ====================================================
  // DRAW THE GAME
  // ====================================================

  display.clearDisplay();

  drawPlayer();
  drawCoin();

  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.print("Score: ");
  display.print(score);

  display.display();

  delay(20);

}
