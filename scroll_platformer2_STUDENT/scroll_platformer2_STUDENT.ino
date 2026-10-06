/*
=========================================================
        RETRO SCROLLING PLATFORMER - PART 2
=========================================================

GOAL

Run, jump and collect stars across a much more dangerous
level! Platforms are now solid on top AND underneath,
some break if you stand on them too long, some move back
and forth or up and down, and falling into a gap means
starting over.

---------------------------------------------------------
CONTROLS

Joystick = LEFT / RIGHT
Button   = JUMP

---------------------------------------------------------
WHAT YOU WILL LEARN

- Solid collision from both above and below (STEP 1)
- Detecting when you've fallen off the level (STEP 2)
- A platform with a timer - it breaks, then comes
  back (STEP 3)
- Collecting items with AABB collision (STEP 4)
- Designing a full level with several platform
  types (STEP 5)

Gravity, jumping, moving platforms and the camera have
already been written for you - same as last time.

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

const int LEVEL_WIDTH = 320;   // 2.5 screens wide

const int PLATFORM_THICKNESS = 4;   // used for every platform type


// ======================================================
// PLAYER
// ======================================================

const int playerStartX = 20;
const int playerStartY = 40;

int playerX = playerStartX;
int playerY = playerStartY;
int playerPrevY = playerStartY;   // where the player was LAST frame

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

int cameraX = 0;

const int cameraDeadZoneLeft = 48;
const int cameraDeadZoneRight = 80;


// ======================================================
// BUTTON
// ======================================================

bool lastButtonState = HIGH;   // HIGH = not pressed (INPUT_PULLUP)


// ======================================================
// CHARACTER (from last lesson - already designed)
// ======================================================

const uint8_t playerFrameA[8] = {
  0b00111100,
  0b01111110,
  0b11011011,
  0b11111111,
  0b00111100,
  0b01111110,
  0b01000010,
  0b10000001
};

const uint8_t playerFrameB[8] = {
  0b00111100,
  0b01111110,
  0b11011011,
  0b11111111,
  0b00111100,
  0b01111110,
  0b00100100,
  0b01011010
};


// ======================================================
// SOLID PLATFORMS
// ======================================================
//
// Used for BOTH the ground and floating platforms - the
// ground is no longer one long strip, it's made of these
// too, with gaps left between them on purpose.
//
// ======================================================

struct Platform
{
  int x;
  int y;
  int width;
};

const int MAX_SOLID = 12;
Platform solidPlatforms[MAX_SOLID];
int solidCount = 0;

void addSolidPlatform(int x, int y, int width)
{
  if (solidCount < MAX_SOLID)
  {
    solidPlatforms[solidCount].x = x;
    solidPlatforms[solidCount].y = y;
    solidPlatforms[solidCount].width = width;
    solidCount++;
  }
}


// ======================================================
// BREAKABLE PLATFORMS
// ======================================================

struct BreakablePlatform
{
  int x;
  int y;
  int width;
  bool broken;
  unsigned long standStart;   // 0 = player is not currently standing on it
  unsigned long breakTime;    // when it broke
};

const int MAX_BREAKABLE = 6;
BreakablePlatform breakablePlatforms[MAX_BREAKABLE];
int breakableCount = 0;

const unsigned long BREAK_DELAY = 500;      // ms standing before it breaks
const unsigned long RESPAWN_DELAY = 3000;   // ms before it comes back

void addBreakablePlatform(int x, int y, int width)
{
  if (breakableCount < MAX_BREAKABLE)
  {
    breakablePlatforms[breakableCount].x = x;
    breakablePlatforms[breakableCount].y = y;
    breakablePlatforms[breakableCount].width = width;
    breakablePlatforms[breakableCount].broken = false;
    breakablePlatforms[breakableCount].standStart = 0;
    breakablePlatforms[breakableCount].breakTime = 0;
    breakableCount++;
  }
}


// ======================================================
// MOVING PLATFORMS
// ======================================================

struct MovingPlatform
{
  int x;
  int y;
  int width;
  int startPos;   // starting x (horizontal) or y (vertical)
  int range;      // how far it travels from startPos
  int speed;
  int axis;        // 0 = horizontal, 1 = vertical
  int direction;    // 1 or -1
};

const int MAX_MOVING = 4;
MovingPlatform movingPlatforms[MAX_MOVING];
int movingCount = 0;

void addMovingPlatform(int x, int y, int width, int axis, int range, int speed)
{
  if (movingCount < MAX_MOVING)
  {
    movingPlatforms[movingCount].x = x;
    movingPlatforms[movingCount].y = y;
    movingPlatforms[movingCount].width = width;
    movingPlatforms[movingCount].startPos = (axis == 0) ? x : y;
    movingPlatforms[movingCount].range = range;
    movingPlatforms[movingCount].speed = speed;
    movingPlatforms[movingCount].axis = axis;
    movingPlatforms[movingCount].direction = 1;
    movingCount++;
  }
}


// ======================================================
// STARS
// ======================================================

struct Star
{
  int x;
  int y;
  bool collected;
};

const int MAX_STARS = 10;
Star stars[MAX_STARS];
int starCount = 0;

int starsCollected = 0;

const int starSize = 5;

void addStar(int x, int y)
{
  if (starCount < MAX_STARS)
  {
    stars[starCount].x = x;
    stars[starCount].y = y;
    stars[starCount].collected = false;
    starCount++;
  }
}


// ======================================================
// GOAL FLAG
// ======================================================

const int flagX = LEVEL_WIDTH - 20;
bool reachedGoal = false;


// ======================================================
// FORWARD DECLARATIONS
// ======================================================
//
// You do NOT need to change this section.
//
// The Arduino IDE tries to guess the prototypes for all
// your functions automatically, but it gets confused by
// functions that take a MovingPlatform or
// BreakablePlatform as a parameter. Declaring them here
// ourselves fixes that.
//
// ======================================================

void moveMovingPlatform(MovingPlatform &plat);
void updateBreakablePlatform(BreakablePlatform &plat);


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
// STEP 1 - SOLID COLLISION (TOP AND BOTTOM)
//
// This one function is used by every platform type, so
// getting it right unlocks everything else this lesson.
//
// Check the player is even above/below this platform
// horizontally first (already written for you):
//
//   playerX + playerWidth > px  &&  playerX < px + pw
//
// LANDING ON TOP (falling down onto it) - same idea as
// last lesson:
//
//   velocityY > 0  AND
//   playerPrevY + playerHeight <= py  AND
//   playerY + playerHeight >= py
//
//   -> playerY = py - playerHeight;
//      velocityY = 0;
//      onGround = true;
//
// BONKING THE BOTTOM (jumping up into it) - the NEW part:
//
//   velocityY < 0  AND
//   playerPrevY >= py + PLATFORM_THICKNESS  AND
//   playerY <= py + PLATFORM_THICKNESS
//
//   -> playerY = py + PLATFORM_THICKNESS;
//      velocityY = 0;
//
// ======================================================

void resolvePlatformCollision(int px, int py, int pw)
{
  bool horizontalOverlap =
    playerX + playerWidth > px &&
    playerX < px + pw;

  if (!horizontalOverlap) return;

  // Write your top and bottom collision checks here.

}

void resolveSolidPlatforms()
{
  for (int i = 0; i < solidCount; i++)
  {
    resolvePlatformCollision(solidPlatforms[i].x, solidPlatforms[i].y, solidPlatforms[i].width);
  }
}


// ======================================================
// MOVING PLATFORMS - MOVE (AND CARRY THE PLAYER)
// ======================================================
//
// You do NOT need to change this section.
//
// This runs BEFORE gravity, using last frame's resting
// position. If the player was standing on this platform,
// it shifts the player by the exact same amount the
// platform just moved - that's what makes it feel like
// you're riding it instead of sliding off (horizontal)
// or falling through it (vertical, moving up).
//
// ======================================================

void moveMovingPlatform(MovingPlatform &plat)
{
  int oldX = plat.x;
  int oldY = plat.y;

  if (plat.axis == 0)
  {
    plat.x += plat.speed * plat.direction;

    if (plat.x > plat.startPos + plat.range || plat.x < plat.startPos)
    {
      plat.direction *= -1;
    }
  }
  else
  {
    plat.y += plat.speed * plat.direction;

    if (plat.y > plat.startPos + plat.range || plat.y < plat.startPos)
    {
      plat.direction *= -1;
    }
  }

  bool playerWasOnTop =
    onGround &&
    playerY + playerHeight == oldY &&
    playerX + playerWidth > oldX &&
    playerX < oldX + plat.width;

  if (playerWasOnTop)
  {
    playerX += (plat.x - oldX);
    playerY += (plat.y - oldY);
  }
}

void moveMovingPlatforms()
{
  for (int i = 0; i < movingCount; i++)
  {
    moveMovingPlatform(movingPlatforms[i]);
  }
}

void resolveMovingPlatformCollisions()
{
  for (int i = 0; i < movingCount; i++)
  {
    resolvePlatformCollision(movingPlatforms[i].x, movingPlatforms[i].y, movingPlatforms[i].width);
  }
}


// ======================================================
// BREAKABLE PLATFORMS - UPDATE
// ======================================================
//
// You do NOT need to change the broken/respawn timer or
// the "is the player standing on it" check below - just
// STEP 3, deciding when it actually breaks.
//
// ======================================================

void updateBreakablePlatform(BreakablePlatform &plat)
{
  if (plat.broken)
  {
    if (millis() - plat.breakTime > RESPAWN_DELAY)
    {
      plat.broken = false;
      plat.standStart = 0;
    }
    return;   // not solid while broken
  }

  resolvePlatformCollision(plat.x, plat.y, plat.width);

  bool standingOnIt =
    playerY + playerHeight == plat.y &&
    playerX + playerWidth > plat.x &&
    playerX < plat.x + plat.width;

  if (!standingOnIt)
  {
    plat.standStart = 0;
    return;
  }

  if (plat.standStart == 0)
  {
    plat.standStart = millis();
  }

  // STEP 3 - BREAK THE PLATFORM
  //
  // The player has now been standing on this platform for
  // (millis() - plat.standStart) ms. Once that passes
  // BREAK_DELAY, break it:
  //
  //   plat.broken = true;
  //   plat.breakTime = millis();

}

void updateBreakablePlatforms()
{
  for (int i = 0; i < breakableCount; i++)
  {
    updateBreakablePlatform(breakablePlatforms[i]);
  }
}


// ======================================================
// STEP 2 - FELL OFF THE LEVEL
//
// If the player has dropped below the bottom of the
// screen (there's no ground under a gap), send them back
// to the start:
//
//   playerX = playerStartX;
//   playerY = playerStartY;
//   velocityY = 0;
//
// Hint: compare playerY to SCREEN_HEIGHT.
//
// ======================================================

void checkFallDeath()
{
  // Write your fall-death check here.

}


// ======================================================
// STEP 4 - COLLECT THE STAR
//
// Same AABB idea as the coin from a previous lesson:
//
//   playerX < stars[i].x + starSize   &&
//   playerX + playerWidth > stars[i].x   &&
//   playerY < stars[i].y + starSize   &&
//   playerY + playerHeight > stars[i].y
//
// If that's true:
//
//   stars[i].collected = true;
//   starsCollected++;
//
// ======================================================

void checkStarCollision()
{
  for (int i = 0; i < starCount; i++)
  {
    if (stars[i].collected) continue;

    // Write your star collection check here.

  }
}


// ======================================================
// DRAWING
// ======================================================
//
// You do NOT need to change this section.
//
// ======================================================

void drawSolidPlatforms()
{
  for (int i = 0; i < solidCount; i++)
  {
    int screenX = solidPlatforms[i].x - cameraX;
    display.fillRect(screenX, solidPlatforms[i].y, solidPlatforms[i].width, PLATFORM_THICKNESS, SSD1306_WHITE);
  }
}

void drawMovingPlatforms()
{
  for (int i = 0; i < movingCount; i++)
  {
    int screenX = movingPlatforms[i].x - cameraX;
    display.fillRect(screenX, movingPlatforms[i].y, movingPlatforms[i].width, PLATFORM_THICKNESS, SSD1306_WHITE);
  }
}

void drawBreakablePlatforms()
{
  for (int i = 0; i < breakableCount; i++)
  {
    BreakablePlatform &plat = breakablePlatforms[i];

    if (plat.broken) continue;

    bool warning = plat.standStart != 0 && (millis() - plat.standStart > BREAK_DELAY - 300);

    if (warning && (millis() / 100) % 2 == 0)
    {
      continue;   // flicker just before it breaks
    }

    int screenX = plat.x - cameraX;
    display.fillRect(screenX, plat.y, plat.width, PLATFORM_THICKNESS, SSD1306_WHITE);
  }
}

void drawStarIcon(int x, int y)
{
  display.drawPixel(x + 2, y,     SSD1306_WHITE);
  display.drawPixel(x,     y + 2, SSD1306_WHITE);
  display.drawPixel(x + 4, y + 2, SSD1306_WHITE);
  display.drawPixel(x + 2, y + 2, SSD1306_WHITE);
  display.drawPixel(x + 2, y + 4, SSD1306_WHITE);
}

void drawStars()
{
  for (int i = 0; i < starCount; i++)
  {
    if (stars[i].collected) continue;
    int screenX = stars[i].x - cameraX;
    drawStarIcon(screenX, stars[i].y);
  }
}

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

void drawHUD()
{
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Stars:");
  display.print(starsCollected);
  display.print("/");
  display.print(starCount);
}


// ======================================================
// CAMERA
// ======================================================
//
// You do NOT need to change this section - same dead
// zone follow as last lesson.
//
// ======================================================

void updateCamera()
{
  int playerScreenX = playerX - cameraX;

  if (playerScreenX < cameraDeadZoneLeft)
  {
    cameraX -= (cameraDeadZoneLeft - playerScreenX);
  }

  if (playerScreenX > cameraDeadZoneRight)
  {
    cameraX += (playerScreenX - cameraDeadZoneRight);
  }

  cameraX = constrain(cameraX, 0, LEVEL_WIDTH - SCREEN_WIDTH);
}


// ======================================================
// BUTTON HELPER
// ======================================================
//
// You do NOT need to change this section.
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
// SETUP
// ======================================================

void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pinMode(JUMP_BUTTON, INPUT_PULLUP);

  display.clearDisplay();
  display.display();


  // ====================================================
  // STEP 5 - DESIGN YOUR LEVEL
  //
  // Build the level using all four tools below. Leave
  // GAPS between ground platforms on purpose - that's
  // where the danger is!
  //
  // addSolidPlatform(x, y, width);
  //
  // addBreakablePlatform(x, y, width);
  //
  // addMovingPlatform(x, y, width, axis, range, speed);
  //   axis: 0 = moves left/right, 1 = moves up/down
  //
  // addStar(x, y);
  //
  // Example ground with a gap in it:
  //
  // addSolidPlatform(0, 56, 60);
  // addSolidPlatform(90, 56, 50);
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

  if (xValue < 400) playerX -= moveSpeed;
  if (xValue > 600) playerX += moveSpeed;

  playerX = constrain(playerX, 0, LEVEL_WIDTH - playerWidth);


  // ====================================================
  // JUMP
  // ====================================================

  if (buttonPressedEdge())
  {
    if (onGround) velocityY = jumpSpeed;
  }


  // ====================================================
  // MOVING PLATFORMS
  //
  // Moved BEFORE gravity so it can carry the player using
  // last frame's resting position, before gravity shifts
  // them for this frame.
  // ====================================================

  moveMovingPlatforms();


  // ====================================================
  // GRAVITY
  // ====================================================

  playerPrevY = playerY;
  onGround = false;
  velocityY += gravity;
  playerY += velocityY;


  // ====================================================
  // COLLISION
  // ====================================================

  resolveSolidPlatforms();
  resolveMovingPlatformCollisions();
  updateBreakablePlatforms();


  // ====================================================
  // FALL DEATH / STARS / CAMERA
  // ====================================================

  checkFallDeath();
  checkStarCollision();
  updateCamera();


  // ====================================================
  // DRAW
  // ====================================================

  display.clearDisplay();

  drawSolidPlatforms();
  drawMovingPlatforms();
  drawBreakablePlatforms();
  drawStars();
  drawFlag();
  drawPlayer();
  checkGoal();
  drawHUD();

  display.display();

  delay(10);
}
