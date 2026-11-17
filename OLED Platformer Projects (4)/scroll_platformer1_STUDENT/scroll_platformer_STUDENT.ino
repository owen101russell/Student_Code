/*
=========================================================
           RETRO SCROLLING PLATFORMER
=========================================================

GOAL

Run and jump your way across a level that's bigger than
the screen! The camera will follow you as you explore.

---------------------------------------------------------
CONTROLS

Joystick = LEFT / RIGHT
Button   = JUMP

---------------------------------------------------------
WHAT YOU WILL LEARN

- Designing a walking character on a pixel grid
- Building a level that's wider than the screen
- A scrolling camera with a "dead zone" (STEP 4)

Gravity, jumping, and platform collision have already
been written for you - same as last time.

=========================================================
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// ======================================================
// PINS
// ======================================================

const int JOY_X = A0;
const int JUMP_BUTTON = 2;   // other leg of the button goes to GND


// ======================================================
// LEVEL SIZE
// ======================================================
//
// The level is wider than the screen - that's what makes
// the camera scrolling worth doing!
//
// ======================================================

const int LEVEL_WIDTH = 320;   // 2.5 screens wide


// ======================================================
// PLAYER
// ======================================================
//
// playerX is the player's position in the LEVEL (world
// space), not on the screen. The camera works out where
// to actually draw them.
//
// ======================================================

int playerX = 20;
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
// CAMERA
// ======================================================
//
// cameraX is how far the level has scrolled.
// screenX = worldX - cameraX for anything you draw.
//
// ======================================================

int cameraX = 0;

const int cameraDeadZoneLeft = 48;
const int cameraDeadZoneRight = 80;


// ======================================================
// BUTTON
// ======================================================

bool lastButtonState = HIGH;   // HIGH = not pressed (INPUT_PULLUP)


// ======================================================
// LEVEL
// ======================================================

struct Platform
{
  int x;
  int y;
  int width;
};

const int MAX_PLATFORMS = 16;

Platform platforms[MAX_PLATFORMS];

int platformCount = 0;

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
// GOAL FLAG
// ======================================================

const int flagX = LEVEL_WIDTH - 20;
bool reachedGoal = false;


// ======================================================
// STEP 1 - DESIGN YOUR CHARACTER (FRAME A)
//
// Each sprite is 8 rows of 8 bits. Change any 0 to a 1
// to light up that pixel.
//
// Reading a row like  0b01111110  left to right:
//
//   0 1 1 1 1 1 1 0
//   .XXXXXX.
//
// Frame A and Frame B should look slightly different
// (like the arms or legs moving) so it animates as you
// walk.
//
// ======================================================

const uint8_t playerFrameA[8] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};


// ======================================================
// STEP 2 - DESIGN YOUR CHARACTER (FRAME B)
// ======================================================

const uint8_t playerFrameB[8] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};


// ======================================================
// DRAW SPRITE
// ======================================================
//
// You do NOT need to change this section.
//
// ======================================================

void drawSprite(int x, int y, const uint8_t sprite[8])
{
  for (int row = 0; row < 8; row++)
  {
    for (int col = 0; col < 8; col++)
    {
      if (bitRead(sprite[row], 7 - col))
      {
        display.drawPixel(x + col, y + row, SSD1306_WHITE);
      }
    }
  }
}


// ======================================================
// DRAW PLATFORMS
// ======================================================
//
// You do NOT need to change this section.
//
// ======================================================

void drawPlatforms()
{
  for (int i = 0; i < platformCount; i++)
  {
    int screenX = platforms[i].x - cameraX;

    display.fillRect(
      screenX,
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
//
// You do NOT need to change this section.
//
// ======================================================

void drawFloor()
{
  display.fillRect(0 - cameraX, 56, LEVEL_WIDTH, 8, SSD1306_WHITE);
}


// ======================================================
// DRAW FLAG
// ======================================================
//
// You do NOT need to change this section.
//
// ======================================================

void drawFlag()
{
  int screenX = flagX - cameraX;

  display.drawFastVLine(screenX, 20, 36, SSD1306_WHITE);
  display.fillTriangle(
    screenX, 20,
    screenX + 8, 24,
    screenX, 28,
    SSD1306_WHITE
  );
}

void checkGoal()
{
  if (playerX + playerWidth >= flagX)
  {
    reachedGoal = true;
  }

  if (reachedGoal)
  {
    display.setTextSize(1);
    display.setCursor(30, 5);
    display.print("GOAL!");
  }
}


// ======================================================
// CHARACTER ANIMATION
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

  int screenX = playerX - cameraX;

  if (animationFrame)
  {
    drawSprite(screenX, playerY, playerFrameA);
  }
  else
  {
    drawSprite(screenX, playerY, playerFrameB);
  }
}


// ======================================================
// STEP 4 - CAMERA FOLLOW (DEAD ZONE)
//
// The camera should only move once the player gets close
// to the edge of the screen - not every single frame.
//
//   0        48          80        128   <- screen x
//   |         |<--dead zone-->|      |
//            player can move freely here
//   outside this band -> camera catches up
//
// Work out the player's SCREEN position:
//
//   int playerScreenX = playerX - cameraX;
//
// If that's less than cameraDeadZoneLeft, the camera has
// fallen behind on the left - shift cameraX left by the
// difference.
//
// If it's more than cameraDeadZoneRight, do the same on
// the right.
//
// Finally, clamp cameraX so it never scrolls past the
// start or end of the level:
//
//   cameraX = constrain(cameraX, 0, LEVEL_WIDTH - SCREEN_WIDTH);
//
// ======================================================

void updateCamera()
{
  // Write your camera-follow code here.

}


// ======================================================
// BUTTON HELPER
// ======================================================
//
// You do NOT need to change this section.
//
// Returns true for ONE frame only, on the moment the
// button is first pressed down.
//
// ======================================================

bool buttonPressedEdge()
{
  bool current = digitalRead(JUMP_BUTTON);
  bool pressed = (lastButtonState == HIGH && current == LOW);
  lastButtonState = current;
  return pressed;
}


// ======================================================
// COLLISION AND PHYSICS
// ======================================================
//
// You do NOT need to change this section.
//
// Gravity, the floor and platform collision are already
// handled for you - it all works the same in world space
// as it did on a single screen.
//
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
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pinMode(JUMP_BUTTON, INPUT_PULLUP);

  display.clearDisplay();
  display.display();


  // ====================================================
  // STEP 3 - DESIGN YOUR LEVEL
  //
  // Add platforms to create your own level. X can now go
  // all the way up to LEVEL_WIDTH (320), not just 128 -
  // spread them out so the player has to explore!
  //
  // Example:
  //
  // addPlatform(50, 40, 25);
  //
  // ====================================================

}


// ======================================================
// MAIN LOOP
// ======================================================

void loop()
{

  // ====================================================
  // MOVE LEFT / RIGHT
  // ====================================================

  int xValue = analogRead(JOY_X);

  if (xValue < 400)
  {
    playerX -= moveSpeed;
  }

  if (xValue > 600)
  {
    playerX += moveSpeed;
  }

  playerX = constrain(playerX, 0, LEVEL_WIDTH - playerWidth);


  // ====================================================
  // JUMP
  // ====================================================

  if (buttonPressedEdge())
  {
    if (onGround)
    {
      velocityY = jumpSpeed;
    }
  }


  // ====================================================
  // UPDATE
  // ====================================================

  updatePlayer();
  updateCamera();


  // ====================================================
  // DRAW
  // ====================================================

  display.clearDisplay();

  drawFloor();
  drawPlatforms();
  drawFlag();
  drawPlayer();
  checkGoal();

  display.display();

  delay(10);
}
