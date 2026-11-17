/*
=========================================================
        RETRO SCROLLING PLATFORMER - PART 3
=========================================================

GOAL

Same level as before, but now it's patrolled! Stomp on
an enemy from above to defeat it, or touch it any other
way and you'll lose a life.

---------------------------------------------------------
CONTROLS

Joystick = LEFT / RIGHT
Button   = JUMP

---------------------------------------------------------
WHAT YOU WILL LEARN

- Enemy patrol movement (STEP 1)
- Stomping an enemy vs. getting hurt (STEP 2)
- What happens when you run out of lives (STEP 3)
- Adding enemies to your level design (STEP 4)

Gravity, platforms, stars, the camera, and a blinking
invincibility effect have already been written for you.

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
// LIVES
// ======================================================

const int startLives = 3;
int lives = startLives;

bool invincible = false;
unsigned long invincibleStart = 0;
const unsigned long invincibleDuration = 1200;   // ms of safety after getting hit


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
// CHARACTER (from a previous lesson - already designed)
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
// ground is made of these too, with gaps left between
// them on purpose.
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
// ENEMIES
// ======================================================
//
// Enemies only patrol horizontally, back and forth along
// a range - the same idea as a horizontal moving
// platform, just walking on the ground instead.
//
// ======================================================

struct Enemy
{
  int x;
  int y;
  int width;
  int height;
  int startPos;
  int range;
  int speed;
  int direction;
  bool alive;
};

const int MAX_ENEMIES = 4;
Enemy enemies[MAX_ENEMIES];
int enemyCount = 0;

void addEnemy(int x, int y, int width, int height, int range, int speed)
{
  if (enemyCount < MAX_ENEMIES)
  {
    enemies[enemyCount].x = x;
    enemies[enemyCount].y = y;
    enemies[enemyCount].width = width;
    enemies[enemyCount].height = height;
    enemies[enemyCount].startPos = x;
    enemies[enemyCount].range = range;
    enemies[enemyCount].speed = speed;
    enemies[enemyCount].direction = 1;
    enemies[enemyCount].alive = true;
    enemyCount++;
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
// functions that take a MovingPlatform, BreakablePlatform
// or Enemy as a parameter. Declaring them here ourselves
// fixes that.
//
// ======================================================

void moveMovingPlatform(MovingPlatform &plat);
void updateBreakablePlatform(BreakablePlatform &plat);
void updateEnemy(Enemy &e);
void checkEnemyCollision(Enemy &e);
void drawEnemy(Enemy &e);


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
// SOLID COLLISION (TOP AND BOTTOM)
// ======================================================
//
// You do NOT need to change this section - this is the
// same top/bottom collision you wrote in a previous
// lesson. Every platform type (and now enemies too) uses
// it.
//
// ======================================================

void resolvePlatformCollision(int px, int py, int pw)
{
  bool horizontalOverlap =
    playerX + playerWidth > px &&
    playerX < px + pw;

  if (!horizontalOverlap) return;

  if (velocityY > 0 &&
      playerPrevY + playerHeight <= py &&
      playerY + playerHeight >= py)
  {
    playerY = py - playerHeight;
    velocityY = 0;
    onGround = true;
  }

  if (velocityY < 0 &&
      playerPrevY >= py + PLATFORM_THICKNESS &&
      playerY <= py + PLATFORM_THICKNESS)
  {
    playerY = py + PLATFORM_THICKNESS;
    velocityY = 0;
  }
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
// You do NOT need to change this section - same
// break/respawn timer as a previous lesson.
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

  if (millis() - plat.standStart > BREAK_DELAY)
  {
    plat.broken = true;
    plat.breakTime = millis();
  }
}

void updateBreakablePlatforms()
{
  for (int i = 0; i < breakableCount; i++)
  {
    updateBreakablePlatform(breakablePlatforms[i]);
  }
}


// ======================================================
// STEP 1 - ENEMY PATROL
//
// Make the enemy walk back and forth - the same idea as
// the horizontal moving platform:
//
//   e.x += e.speed * e.direction;
//
// Then flip direction when it reaches either end of its
// patrol range:
//
//   if (e.x > e.startPos + e.range || e.x < e.startPos)
//   {
//     e.direction *= -1;
//   }
//
// ======================================================

void updateEnemy(Enemy &e)
{
  if (!e.alive) return;

  // Write your patrol logic here.

}

void updateEnemies()
{
  for (int i = 0; i < enemyCount; i++)
  {
    updateEnemy(enemies[i]);
  }
}


// ======================================================
// STEP 2 - STOMP OR GET HIT
//
// "stomped" is true when the player was falling and
// landed on TOP of the enemy - the same idea as landing
// on a platform.
//
// If stomped, defeat the enemy and give a little bounce:
//
//   e.alive = false;
//   velocityY = jumpSpeed / 2;
//
// Otherwise, the player touched it from the side or
// underneath - they get hurt instead:
//
//   lives--;
//   invincible = true;
//   invincibleStart = millis();
//   playerX = playerStartX;
//   playerY = playerStartY;
//   velocityY = 0;
//
// ======================================================

void checkEnemyCollision(Enemy &e)
{
  if (!e.alive || invincible) return;

  bool touching =
    playerX + playerWidth > e.x &&
    playerX < e.x + e.width &&
    playerY + playerHeight > e.y &&
    playerY < e.y + e.height;

  if (!touching) return;

  bool stomped = velocityY > 0 && playerPrevY + playerHeight <= e.y;

  // Write what happens here.

}

void checkEnemyCollisions()
{
  for (int i = 0; i < enemyCount; i++)
  {
    checkEnemyCollision(enemies[i]);
  }
}


// ======================================================
// FELL OFF THE LEVEL
// ======================================================
//
// You do NOT need to change this section.
//
// ======================================================

void checkFallDeath()
{
  if (playerY > SCREEN_HEIGHT)
  {
    playerX = playerStartX;
    playerY = playerStartY;
    velocityY = 0;
  }
}


// ======================================================
// COLLECT THE STAR
// ======================================================
//
// You do NOT need to change this section.
//
// ======================================================

void checkStarCollision()
{
  for (int i = 0; i < starCount; i++)
  {
    if (stars[i].collected) continue;

    bool touching =
      playerX < stars[i].x + starSize &&
      playerX + playerWidth > stars[i].x &&
      playerY < stars[i].y + starSize &&
      playerY + playerHeight > stars[i].y;

    if (touching)
    {
      stars[i].collected = true;
      starsCollected++;
    }
  }
}


// ======================================================
// STEP 3 - OUT OF LIVES
//
// If lives has dropped to 0 (or below), start over:
//
//   lives = startLives;
//   playerX = playerStartX;
//   playerY = playerStartY;
//   velocityY = 0;
//
// ======================================================

void checkLives()
{
  // Write your check here.

}


// ======================================================
// INVINCIBILITY
// ======================================================
//
// You do NOT need to change this section.
//
// ======================================================

void updateInvincibility()
{
  if (invincible && millis() - invincibleStart > invincibleDuration)
  {
    invincible = false;
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

void drawEnemy(Enemy &e)
{
  if (!e.alive) return;

  int screenX = e.x - cameraX;
  display.fillRect(screenX, e.y, e.width, e.height, SSD1306_WHITE);
  display.drawPixel(screenX + 1, e.y + 1, SSD1306_BLACK);   // a little eye
}

void drawEnemies()
{
  for (int i = 0; i < enemyCount; i++)
  {
    drawEnemy(enemies[i]);
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
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(45, 28);
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

  if (invincible && (millis() / 150) % 2 == 0)
  {
    return;   // skip drawing this frame - makes the player flash
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
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Stars:");
  display.print(starsCollected);
  display.print("/");
  display.print(starCount);

  display.setCursor(90, 0);
  display.print("Life:");
  display.print(lives);
}


// ======================================================
// CAMERA
// ======================================================
//
// You do NOT need to change this section.
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
  // STEP 4 - DESIGN YOUR LEVEL
  //
  // Everything from before still works the same. Now you
  // can also add enemies:
  //
  // addEnemy(x, y, width, height, range, speed);
  //
  // Enemies patrol back and forth just like a horizontal
  // moving platform - keep their range inside a solid
  // platform so they don't walk off the edge!
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
  // ENEMIES
  // ====================================================

  updateEnemies();
  checkEnemyCollisions();


  // ====================================================
  // FALL DEATH / STARS / LIVES / CAMERA
  // ====================================================

  checkFallDeath();
  checkStarCollision();
  checkLives();
  updateInvincibility();
  updateCamera();


  // ====================================================
  // DRAW
  // ====================================================

  display.clearDisplay();

  drawSolidPlatforms();
  drawMovingPlatforms();
  drawBreakablePlatforms();
  drawStars();
  drawEnemies();
  drawFlag();
  drawPlayer();
  checkGoal();
  drawHUD();

  display.display();

  delay(10);
}
