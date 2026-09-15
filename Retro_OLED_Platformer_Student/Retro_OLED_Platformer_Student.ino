/*
=========================================================
              RETRO OLED PLATFORMER
=========================================================

GOAL

Create your own tiny retro platformer!

Make your own character, control it with the
button matrix, and design your own level.

---------------------------------------------------------
CONTROLS

4 = LEFT
6 = RIGHT
8 = JUMP

---------------------------------------------------------
WHAT YOU WILL LEARN

• Creating simple animations
• Using functions
• Designing a level
• Working with a button matrix

The difficult physics and collision code has already
been written for you.

=========================================================
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// ======================================================
// BUTTON MATRIX
// ======================================================

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] =
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {3, 4, 5, 6};
byte colPins[COLS] = {7, 8, 9, 10};

Keypad keypad = Keypad(
  makeKeymap(keys),
  rowPins,
  colPins,
  ROWS,
  COLS
);


// ======================================================
// PLAYER
// ======================================================

int playerX = 10;
int playerY = 40;

const int playerWidth = 8;
const int playerHeight = 8;

const int moveSpeed = 2;


// ======================================================
// PHYSICS
// ======================================================

int velocityY = 0;

const int gravity = 1;
const int jumpSpeed = -6;

bool onGround = false;


// ======================================================
// LEVEL
// ======================================================

struct Platform
{
  int x;
  int y;
  int width;
};

const int MAX_PLATFORMS = 10;

Platform platforms[MAX_PLATFORMS];

int platformCount = 0;


// ======================================================
// ADD PLATFORM
// ======================================================
//
// You will use this function to design your level.
//
// Example:
//
// addPlatform(20, 40, 30);
//
// 20 = X position
// 40 = Y position
// 30 = Width
//
// ======================================================

void addPlatform(int x, int y, int width)
{
  if (platformCount < MAX_PLATFORMS)
  {
    platforms[platformCount].x = x;
    platforms[platformCount].y = y;
    platforms[platformCount].width = width;

    platformCount++;
  }
}


// ======================================================
// DRAW PLATFORMS
// ======================================================

void drawPlatforms()
{
  for (int i = 0; i < platformCount; i++)
  {
    display.fillRect(
      platforms[i].x,
      platforms[i].y,
      platforms[i].width,
      3,
      SSD1306_WHITE
    );
  }
}


// ======================================================
// DRAW FLOOR
// ======================================================

void drawFloor()
{
  display.fillRect(
    0,
    56,
    SCREEN_WIDTH,
    8,
    SSD1306_WHITE
  );
}


// ======================================================
// CHARACTER ANIMATION
// ======================================================
//
// STEP 1
//
// Design your FIRST character frame.
//
// You already know how to use functions such as:
//
// display.fillRect()
// display.drawRect()
// display.drawCircle()
// display.drawLine()
// display.drawPixel()
//
// Your character should fit inside an 8 x 8 area.
//
// The character's top-left corner is:
// x, y
//
// Try making a recognisable character.
//
// ======================================================

void drawPlayerFrame1(int x, int y)
{
  // Your character's first frame goes here.
}



// ======================================================
// STEP 2
//
// Design your SECOND character frame.
//
// Change the arms, legs, body or another part of
// your character so that it looks like the character
// is moving.
//
// Keep the character inside an 8 x 8 area.
//
// ======================================================

void drawPlayerFrame2(int x, int y)
{
  // Your character's second frame goes here.
}


// ======================================================
// CHARACTER ANIMATION SYSTEM
// ======================================================
//
// You do NOT need to change this section.
//
// It automatically switches between your two frames.
//
// ======================================================

unsigned long lastAnimationTime = 0;

bool animationFrame = false;

void drawPlayer()
{
  if (millis() - lastAnimationTime > 120)
  {
    animationFrame = !animationFrame;
    lastAnimationTime = millis();
  }

  if (animationFrame)
  {
    drawPlayerFrame1(playerX, playerY);
  }
  else
  {
    drawPlayerFrame2(playerX, playerY);
  }
}


// ======================================================
// COLLISION AND PHYSICS
// ======================================================
//
// You do NOT need to change this section.
//
// Gravity, jumping, the floor and platform collision
// are already handled for you.
// ======================================================

void updatePlayer()
{
  int oldY = playerY;

  onGround = false;

  velocityY += gravity;

  playerY += velocityY;


  // ----------------------------------------------------
  // FLOOR COLLISION
  // ----------------------------------------------------

  if (playerY + playerHeight >= 56)
  {
    playerY = 56 - playerHeight;
    velocityY = 0;
    onGround = true;
  }


  // ----------------------------------------------------
  // PLATFORM COLLISION
  // ----------------------------------------------------

  if (velocityY >= 0)
  {
    for (int i = 0; i < platformCount; i++)
    {
      int platformX = platforms[i].x;
      int platformY = platforms[i].y;
      int platformWidth = platforms[i].width;

      bool horizontalOverlap =
        playerX + playerWidth > platformX &&
        playerX < platformX + platformWidth;

      bool crossedPlatform =
        oldY + playerHeight <= platformY &&
        playerY + playerHeight >= platformY;

      if (horizontalOverlap && crossedPlatform)
      {
        playerY = platformY - playerHeight;
        velocityY = 0;
        onGround = true;
      }
    }
  }
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

  display.clearDisplay();
  display.display();


  // ====================================================
  // STEP 3 — DESIGN YOUR LEVEL
  // ====================================================
  //
  // Add platforms to create your own level.
  //
  // Example:
  //
  // addPlatform(10, 45, 25);
  //
  // Try changing the numbers.
  //
  // Remember:
  //
  // X gets bigger ---------->
  //
  // Y gets bigger
  //        |
  //        |
  //        V
  //
  // Try creating a level that the player can actually
  // jump through.
  //
  // ====================================================

  addPlatform(5, 48, 30);
  addPlatform(50, 38, 25);
  addPlatform(85, 28, 30);
  addPlatform(35, 18, 20);
}


// ======================================================
// MAIN LOOP
// ======================================================

void loop()
{

  // ====================================================
  // MOVE LEFT
  // ====================================================

  if (keypad.isPressed('4'))
  {
    playerX -= moveSpeed;
  }


  // ====================================================
  // MOVE RIGHT
  // ====================================================

  if (keypad.isPressed('6'))
  {
    playerX += moveSpeed;
  }


  // ====================================================
  // KEEP PLAYER ON SCREEN
  // ====================================================

  playerX = constrain(
    playerX,
    0,
    SCREEN_WIDTH - playerWidth
  );


  // ====================================================
  // JUMP
  // ====================================================

  if (keypad.getKey() == '8')
  {
    if (onGround)
    {
      velocityY = jumpSpeed;
    }
  }


  // ====================================================
  // UPDATE PHYSICS
  // ====================================================

  updatePlayer();


  // ====================================================
  // DRAW
  // ====================================================

  display.clearDisplay();

  drawFloor();

  drawPlatforms();

  drawPlayer();

  display.display();

  delay(10);
}
