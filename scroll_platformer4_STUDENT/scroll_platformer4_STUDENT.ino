/*
=========================================================
        RETRO SCROLLING PLATFORMER - PART 4
=========================================================

GOAL

One last addition: spikes that hurt on contact (no
stomping these!) and a dash so you can zip past danger
or just move faster.

---------------------------------------------------------
CONTROLS

Joystick    = LEFT / RIGHT
Jump button = JUMP
Dash button = DASH (short burst of speed, has a cooldown)

---------------------------------------------------------
WHAT YOU WILL LEARN

- What happens when you touch a spike (STEP 1)
- Starting a dash on a button press (STEP 2)

Everything else - platforms, enemies, stars, lives,
the camera - is exactly as you left it.

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
const int JUMP_BUTTON = 2;
const int DASH_BUTTON = 3;


// ======================================================
// LEVEL SIZE
// ======================================================

const int LEVEL_WIDTH = 320;
const int PLATFORM_THICKNESS = 4;


// ======================================================
// PLAYER
// ======================================================

const int playerStartX = 20;
const int playerStartY = 40;

int playerX = playerStartX;
int playerY = playerStartY;
int playerPrevY = playerStartY;

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
// LIVES AND INVINCIBILITY
// ======================================================

const int startLives = 3;
int lives = startLives;

bool invincible = false;
unsigned long invincibleStart = 0;

const unsigned long invincibleDuration = 1200;

// Prevents the game from collecting stars or processing
// further deaths on the same frame as a death.
bool playerDiedThisFrame = false;


// ======================================================
// DASH
// ======================================================

bool lastDashButtonState = HIGH;

bool dashActive = false;
unsigned long dashStart = 0;
const unsigned long dashDuration = 200;          // how long a dash lasts

bool dashOnCooldown = false;
unsigned long dashCooldownStart = 0;
const unsigned long dashCooldownDuration = 2000; // how long until you can dash again

const int dashBonusSpeed = 4;                    // extra speed added while dashing


// ======================================================
// CAMERA
// ======================================================

int cameraX = 0;

const int cameraDeadZoneLeft = 48;
const int cameraDeadZoneRight = 80;


// ======================================================
// BUTTON
// ======================================================

bool lastButtonState = HIGH;


// ======================================================
// PLAYER ANIMATION
// ======================================================

unsigned long lastAnimationTime = 0;
bool animationFrame = false;

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

  unsigned long standStart;
  unsigned long breakTime;
};

const int MAX_BREAKABLE = 6;

BreakablePlatform breakablePlatforms[MAX_BREAKABLE];
int breakableCount = 0;

const unsigned long BREAK_DELAY = 500;
const unsigned long RESPAWN_DELAY = 3000;

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

  int startPos;
  int range;
  int speed;

  int axis;       // 0 = horizontal, 1 = vertical
  int direction;  // 1 or -1
};

const int MAX_MOVING = 4;

MovingPlatform movingPlatforms[MAX_MOVING];
int movingCount = 0;

void addMovingPlatform(
  int x,
  int y,
  int width,
  int axis,
  int range,
  int speed
)
{
  if (movingCount < MAX_MOVING)
  {
    movingPlatforms[movingCount].x = x;
    movingPlatforms[movingCount].y = y;
    movingPlatforms[movingCount].width = width;

    movingPlatforms[movingCount].startPos =
      (axis == 0) ? x : y;

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

void addEnemy(
  int x,
  int y,
  int width,
  int height,
  int range,
  int speed
)
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
// SPIKES
// ======================================================
//
// Spikes don't move and don't care how you touch them -
// unlike an enemy, there's no "stomp" exception. They sit
// on top of a solid surface, so their own height is fixed.
//
// ======================================================

const int spikeHeight = 4;

struct Spike
{
  int x;
  int y;
  int width;
};

const int MAX_SPIKES = 8;

Spike spikes[MAX_SPIKES];
int spikeCount = 0;

void addSpike(int x, int y, int width)
{
  if (spikeCount < MAX_SPIKES)
  {
    spikes[spikeCount].x = x;
    spikes[spikeCount].y = y;
    spikes[spikeCount].width = width;

    spikeCount++;
  }
}


// ======================================================
// GOAL
// ======================================================

const int flagX = LEVEL_WIDTH - 20;

bool reachedGoal = false;


// ======================================================
// FORWARD DECLARATIONS
// ======================================================

void moveMovingPlatform(MovingPlatform &plat);
void updateBreakablePlatform(BreakablePlatform &plat);

void updateEnemy(Enemy &e);
void checkEnemyCollision(Enemy &e);
void drawEnemy(Enemy &e);

void resetGame();
void handlePlayerDeath();

void resolvePlatformCollision(int px, int py, int pw);


// ======================================================
// RESET THE ENTIRE GAME
// ======================================================
//
// Called when the player runs out of lives.
//
// Resets the player, stars, enemies, moving platforms,
// breakable platforms, dash, camera, and lives.
//
// ======================================================

void resetGame()
{
  // Reset player position and physics.

  playerX = playerStartX;
  playerY = playerStartY;
  playerPrevY = playerStartY;

  velocityY = 0;
  onGround = false;

  // Restore lives and invincibility.

  lives = startLives;

  invincible = false;
  invincibleStart = 0;

  // Clear any in-progress dash.

  dashActive = false;
  dashOnCooldown = false;

  // Return camera to the beginning.

  cameraX = 0;

  // Reset collected stars.

  starsCollected = 0;

  for (int i = 0; i < starCount; i++)
  {
    stars[i].collected = false;
  }

  // Restore every enemy to its starting position.

  for (int i = 0; i < enemyCount; i++)
  {
    enemies[i].x = enemies[i].startPos;
    enemies[i].direction = 1;
    enemies[i].alive = true;
  }

  // Return moving platforms to their starting positions.

  for (int i = 0; i < movingCount; i++)
  {
    if (movingPlatforms[i].axis == 0)
    {
      movingPlatforms[i].x = movingPlatforms[i].startPos;
    }
    else
    {
      movingPlatforms[i].y = movingPlatforms[i].startPos;
    }

    movingPlatforms[i].direction = 1;
  }

  // Restore all breakable platforms.

  for (int i = 0; i < breakableCount; i++)
  {
    breakablePlatforms[i].broken = false;
    breakablePlatforms[i].standStart = 0;
    breakablePlatforms[i].breakTime = 0;
  }

  // Reset the finish state.

  reachedGoal = false;

  // Restart player animation.

  animationFrame = false;
  lastAnimationTime = millis();

  // Synchronise the button states after resetting.

  lastButtonState = digitalRead(JUMP_BUTTON);
  lastDashButtonState = digitalRead(DASH_BUTTON);
}


// ======================================================
// SHARED PLAYER DEATH LOGIC
// ======================================================
//
// Falling off the map, getting hit by an enemy, AND now
// touching a spike all use this function.
//
// Losing a life respawns the player at the start.
//
// Losing the final life resets the entire game.
//
// ======================================================

void handlePlayerDeath()
{
  playerDiedThisFrame = true;

  lives--;

  // If all lives are lost, restart the entire game.

  if (lives <= 0)
  {
    resetGame();

    // Give the player temporary protection at the
    // beginning of the restarted game.

    invincible = true;
    invincibleStart = millis();

    // Keep this true for the remainder of this frame.

    playerDiedThisFrame = true;

    return;
  }

  // Otherwise, respawn the player.

  playerX = playerStartX;
  playerY = playerStartY;
  playerPrevY = playerStartY;

  velocityY = 0;
  onGround = false;

  // Return the camera to the beginning.

  cameraX = 0;

  // Briefly protect the player after respawning.

  invincible = true;
  invincibleStart = millis();
}


// ======================================================
// DRAW PLAYER SPRITE
// ======================================================

void drawSprite(int x, int y, const uint8_t sprite[8])
{
  for (int row = 0; row < 8; row++)
  {
    for (int col = 0; col < 8; col++)
    {
      if (bitRead(sprite[row], 7 - col))
      {
        display.drawPixel(
          x + col,
          y + row,
          SSD1306_WHITE
        );
      }
    }
  }
}


// ======================================================
// PLATFORM COLLISION
// ======================================================

void resolvePlatformCollision(int px, int py, int pw)
{
  bool horizontalOverlap =
    playerX + playerWidth > px &&
    playerX < px + pw;

  if (!horizontalOverlap) return;

  // Landing on the platform.

  if (velocityY > 0 &&
      playerPrevY + playerHeight <= py &&
      playerY + playerHeight >= py)
  {
    playerY = py - playerHeight;

    velocityY = 0;
    onGround = true;
  }

  // Hitting the underside of the platform.

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
    resolvePlatformCollision(
      solidPlatforms[i].x,
      solidPlatforms[i].y,
      solidPlatforms[i].width
    );
  }
}


// ======================================================
// MOVING PLATFORMS
// ======================================================

void moveMovingPlatform(MovingPlatform &plat)
{
  int oldX = plat.x;
  int oldY = plat.y;

  if (plat.axis == 0)
  {
    plat.x += plat.speed * plat.direction;

    if (plat.x > plat.startPos + plat.range ||
        plat.x < plat.startPos)
    {
      plat.direction *= -1;
    }
  }
  else
  {
    plat.y += plat.speed * plat.direction;

    if (plat.y > plat.startPos + plat.range ||
        plat.y < plat.startPos)
    {
      plat.direction *= -1;
    }
  }

  // Carry the player if they were standing on the
  // platform before it moved.

  bool playerWasOnTop =
    onGround &&
    playerY + playerHeight == oldY &&
    playerX + playerWidth > oldX &&
    playerX < oldX + plat.width;

  if (playerWasOnTop)
  {
    playerX += plat.x - oldX;
    playerY += plat.y - oldY;
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
    resolvePlatformCollision(
      movingPlatforms[i].x,
      movingPlatforms[i].y,
      movingPlatforms[i].width
    );
  }
}


// ======================================================
// BREAKABLE PLATFORMS
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

    return;
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
// ENEMY MOVEMENT
// ======================================================

void updateEnemy(Enemy &e)
{
  if (!e.alive) return;

  e.x += e.speed * e.direction;

  // Reverse direction at the patrol boundaries.

  if (e.x >= e.startPos + e.range)
  {
    e.x = e.startPos + e.range;
    e.direction = -1;
  }
  else if (e.x <= e.startPos)
  {
    e.x = e.startPos;
    e.direction = 1;
  }
}


void updateEnemies()
{
  for (int i = 0; i < enemyCount; i++)
  {
    updateEnemy(enemies[i]);
  }
}


// ======================================================
// ENEMY COLLISION: STOMP OR GET HIT
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

  // The player must be falling and have been above
  // the enemy during the previous frame to stomp it.

  bool stomped =
    velocityY > 0 &&
    playerPrevY + playerHeight <= e.y;

  if (stomped)
  {
    // Defeat the enemy.

    e.alive = false;

    // Place the player directly on top of the enemy
    // and bounce them upwards.

    playerY = e.y - playerHeight;
    velocityY = jumpSpeed / 2;

    onGround = false;
  }
  else
  {
    // Contact from the side or underneath costs a life.

    handlePlayerDeath();
  }
}


void checkEnemyCollisions()
{
  if (invincible) return;

  for (int i = 0; i < enemyCount; i++)
  {
    if (!enemies[i].alive) continue;

    checkEnemyCollision(enemies[i]);

    // Process no more than one enemy collision per frame.

    if (playerDiedThisFrame || !enemies[i].alive)
    {
      return;
    }
  }
}


// ======================================================
// STEP 1 - SPIKE COLLISION
//
// Unlike an enemy, a spike has no stomp exception - any
// contact at all should hurt. "touching" below is the
// same AABB overlap check you've used before.
//
// If touching is true, call the same death function
// everything else uses, then stop checking (one death
// is enough for this frame):
//
//   handlePlayerDeath();
//   return;
//
// ======================================================

void checkSpikeCollisions()
{
  if (invincible) return;

  for (int i = 0; i < spikeCount; i++)
  {
    bool touching =
      playerX + playerWidth > spikes[i].x &&
      playerX < spikes[i].x + spikes[i].width &&
      playerY + playerHeight > spikes[i].y &&
      playerY < spikes[i].y + spikeHeight;

    // Write what happens here.

  }
}


// ======================================================
// FALLING OFF THE MAP
// ======================================================
//
// Falling, spikes, and enemy damage now all use exactly
// the same life-losing and respawn function.
//
// ======================================================

void checkFallDeath()
{
  if (playerY > SCREEN_HEIGHT)
  {
    handlePlayerDeath();
  }
}


// ======================================================
// COLLECT STARS
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
// INVINCIBILITY TIMER
// ======================================================

void updateInvincibility()
{
  if (invincible &&
      millis() - invincibleStart > invincibleDuration)
  {
    invincible = false;
  }
}


// ======================================================
// STEP 2 - START A DASH
//
// The cooldown-expiry checks below are already written.
// You just need to START a dash.
//
// If the dash button was just pressed (use
// dashButtonPressedEdge()) AND dash is NOT already on
// cooldown, kick one off:
//
//   dashActive = true;
//   dashStart = millis();
//   dashOnCooldown = true;
//   dashCooldownStart = millis();
//
// ======================================================

void handleDash()
{
  // Write your dash-start logic here.


  if (dashActive && millis() - dashStart > dashDuration)
  {
    dashActive = false;
  }

  if (dashOnCooldown && millis() - dashCooldownStart > dashCooldownDuration)
  {
    dashOnCooldown = false;
  }
}


// ======================================================
// GOAL CONDITION
// ======================================================
//
// Reaching the flag is only enough to win if every star
// has been collected.
//
// ======================================================

void updateGoal()
{
  bool atEnd =
    playerX + playerWidth >= flagX;

  bool allStarsCollected =
    starsCollected >= starCount;

  if (atEnd && allStarsCollected)
  {
    reachedGoal = true;
  }
}


// ======================================================
// DRAW SOLID PLATFORMS
// ======================================================

void drawSolidPlatforms()
{
  for (int i = 0; i < solidCount; i++)
  {
    int screenX = solidPlatforms[i].x - cameraX;

    display.fillRect(
      screenX,
      solidPlatforms[i].y,
      solidPlatforms[i].width,
      PLATFORM_THICKNESS,
      SSD1306_WHITE
    );
  }
}


// ======================================================
// DRAW MOVING PLATFORMS
// ======================================================

void drawMovingPlatforms()
{
  for (int i = 0; i < movingCount; i++)
  {
    int screenX = movingPlatforms[i].x - cameraX;

    display.fillRect(
      screenX,
      movingPlatforms[i].y,
      movingPlatforms[i].width,
      PLATFORM_THICKNESS,
      SSD1306_WHITE
    );
  }
}


// ======================================================
// DRAW BREAKABLE PLATFORMS
// ======================================================

void drawBreakablePlatforms()
{
  for (int i = 0; i < breakableCount; i++)
  {
    BreakablePlatform &plat = breakablePlatforms[i];

    if (plat.broken) continue;

    bool warning =
      plat.standStart != 0 &&
      millis() - plat.standStart > BREAK_DELAY - 300;

    if (warning && (millis() / 100) % 2 == 0)
    {
      continue;
    }

    int screenX = plat.x - cameraX;

    display.fillRect(
      screenX,
      plat.y,
      plat.width,
      PLATFORM_THICKNESS,
      SSD1306_WHITE
    );
  }
}


// ======================================================
// DRAW STARS
// ======================================================

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


// ======================================================
// DRAW SPIKES
// ======================================================
//
// Each spike patch is drawn as a row of little triangle
// teeth, however wide it was created.
//
// ======================================================

void drawSpikes()
{
  const int toothWidth = 4;

  for (int i = 0; i < spikeCount; i++)
  {
    int screenX = spikes[i].x - cameraX;
    int teeth = spikes[i].width / toothWidth;

    for (int t = 0; t < teeth; t++)
    {
      int tx = screenX + t * toothWidth;

      display.fillTriangle(
        tx, spikes[i].y + spikeHeight,
        tx + toothWidth / 2, spikes[i].y,
        tx + toothWidth, spikes[i].y + spikeHeight,
        SSD1306_WHITE
      );
    }
  }
}


// ======================================================
// IMPROVED ENEMY SPRITE
// ======================================================
//
// Rounded body, antennae, eyes, mouth and feet.
//
// ======================================================

void drawEnemy(Enemy &e)
{
  if (!e.alive) return;

  int screenX = e.x - cameraX;

  // Antennae.

  display.drawPixel(
    screenX + 2,
    e.y,
    SSD1306_WHITE
  );

  display.drawPixel(
    screenX + 7,
    e.y,
    SSD1306_WHITE
  );

  display.drawLine(
    screenX + 2,
    e.y + 1,
    screenX + 3,
    e.y + 2,
    SSD1306_WHITE
  );

  display.drawLine(
    screenX + 7,
    e.y + 1,
    screenX + 6,
    e.y + 2,
    SSD1306_WHITE
  );

  // Rounded body.

  display.fillRoundRect(
    screenX + 1,
    e.y + 2,
    e.width - 2,
    e.height - 3,
    2,
    SSD1306_WHITE
  );

  // Eyes.

  display.drawPixel(
    screenX + 3,
    e.y + 3,
    SSD1306_BLACK
  );

  display.drawPixel(
    screenX + 6,
    e.y + 3,
    SSD1306_BLACK
  );

  // Mouth.

  display.drawFastHLine(
    screenX + 4,
    e.y + 5,
    2,
    SSD1306_BLACK
  );

  // Little feet.

  display.drawPixel(
    screenX + 1,
    e.y + e.height - 1,
    SSD1306_WHITE
  );

  display.drawPixel(
    screenX + 3,
    e.y + e.height - 1,
    SSD1306_WHITE
  );

  display.drawPixel(
    screenX + 6,
    e.y + e.height - 1,
    SSD1306_WHITE
  );

  display.drawPixel(
    screenX + 8,
    e.y + e.height - 1,
    SSD1306_WHITE
  );
}


void drawEnemies()
{
  for (int i = 0; i < enemyCount; i++)
  {
    drawEnemy(enemies[i]);
  }
}


// ======================================================
// DRAW FLAG
// ======================================================

void drawFlag()
{
  int screenX = flagX - cameraX;

  display.drawFastVLine(
    screenX,
    20,
    36,
    SSD1306_WHITE
  );

  display.fillTriangle(
    screenX,     20,
    screenX + 8, 24,
    screenX,     28,
    SSD1306_WHITE
  );
}


// ======================================================
// GOAL AND VICTORY MESSAGES
// ======================================================

void drawGoalMessage()
{
  bool atEnd =
    playerX + playerWidth >= flagX;

  // Victory screen.

  if (reachedGoal)
  {
    display.fillRect(
      18, 21,
      92, 26,
      SSD1306_BLACK
    );

    display.drawRect(
      18, 21,
      92, 26,
      SSD1306_WHITE
    );

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(40, 26);
    display.print("YOU WIN!");

    display.setCursor(22, 36);
    display.print("LEVEL COMPLETE");
  }

  // The player reached the flag without collecting
  // every star. Let them know they must go back.

  else if (atEnd && starsCollected < starCount)
  {
    display.fillRect(
      16, 25,
      96, 14,
      SSD1306_BLACK
    );

    display.drawRect(
      16, 25,
      96, 14,
      SSD1306_WHITE
    );

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(25, 29);
    display.print("GET ALL STARS");
  }
}


// ======================================================
// DRAW PLAYER
// ======================================================

void drawPlayer()
{
  if (millis() - lastAnimationTime > 120)
  {
    animationFrame = !animationFrame;
    lastAnimationTime = millis();
  }

  // Blink during the invincibility period.

  if (invincible && (millis() / 150) % 2 == 0)
  {
    return;
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
// DRAW DASH BAR
// ======================================================
//
// A small bar in the bottom-left corner (below the
// ground, so it never overlaps the level). Filling up
// means dash is on cooldown; a full bar means it's ready.
//
// ======================================================

void drawDashBar()
{
  const int barWidth = 16;
  const int barHeight = 3;
  int barX = ((playerX-cameraX) + 4) - (barWidth/2);
  int barY = (playerY - 4) - barHeight;
  

  display.drawRect(barX, barY, barWidth, barHeight, SSD1306_WHITE);

  int fillWidth;

  if (dashOnCooldown)
  {
    unsigned long elapsed = millis() - dashCooldownStart;
    fillWidth = map(elapsed, 0, dashCooldownDuration, 0, barWidth);
    fillWidth = constrain(fillWidth, 0, barWidth);
  }
  else
  {
    fillWidth = barWidth;
  }

  if (fillWidth > 0)
  {
    display.fillRect(barX, barY, fillWidth, barHeight, SSD1306_WHITE);
  }
}


// ======================================================
// DRAW HUD
// ======================================================

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

void updateCamera()
{
  int playerScreenX = playerX - cameraX;

  if (playerScreenX < cameraDeadZoneLeft)
  {
    cameraX -= cameraDeadZoneLeft - playerScreenX;
  }

  if (playerScreenX > cameraDeadZoneRight)
  {
    cameraX += playerScreenX - cameraDeadZoneRight;
  }

  cameraX = constrain(
    cameraX,
    0,
    LEVEL_WIDTH - SCREEN_WIDTH
  );
}


// ======================================================
// BUTTON HELPERS
// ======================================================

bool buttonPressedEdge()
{
  bool current = digitalRead(JUMP_BUTTON);

  bool pressed =
    lastButtonState == HIGH &&
    current == LOW;

  lastButtonState = current;

  return pressed;
}


bool dashButtonPressedEdge()
{
  bool current = digitalRead(DASH_BUTTON);

  bool pressed =
    lastDashButtonState == HIGH &&
    current == LOW;

  lastDashButtonState = current;

  return pressed;
}


// ======================================================
// SETUP: BUILD THE LEVEL
// ======================================================

void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  pinMode(JUMP_BUTTON, INPUT_PULLUP);
  pinMode(DASH_BUTTON, INPUT_PULLUP);

  display.clearDisplay();
  display.display();


  // ----------------------------------------------------
  // SOLID PLATFORMS
  // ----------------------------------------------------

  addSolidPlatform(0, 56, 60);
  addSolidPlatform(90, 56, 50);
  addSolidPlatform(160, 56, 40);
  addSolidPlatform(230, 56, 90);

  addSolidPlatform(70, 44, 20);
  addSolidPlatform(145, 35, 20);


  // ----------------------------------------------------
  // BREAKABLE PLATFORM
  // ----------------------------------------------------

  addBreakablePlatform(205, 45, 20);


  // ----------------------------------------------------
  // MOVING PLATFORMS
  // ----------------------------------------------------

  addMovingPlatform(102, 32, 16, 0, 20, 2);
  addMovingPlatform(250, 20, 16, 1, 26, 1);


  // ----------------------------------------------------
  // STARS
  // ----------------------------------------------------

  addStar(75, 30);
  addStar(150, 25);
  addStar(210, 35);
  addStar(255, 15);
  addStar(300, 40);


  // ----------------------------------------------------
  // ENEMIES
  //
  // Wider and taller sprites to allow room for their
  // eyes, antennae, mouth and feet.
  // ----------------------------------------------------

  addEnemy(100, 48, 10, 8, 30, 1);
  addEnemy(240, 48, 10, 8, 40, 1);


  // ----------------------------------------------------
  // SPIKES
  //
  // Add your own with addSpike(x, y, width) - they sit
  // flush on top of the ground (y = groundTop - 4).
  // ----------------------------------------------------


  // ----------------------------------------------------
  // INITIALISE THE GAME STATE
  // ----------------------------------------------------

  resetGame();
}


// ======================================================
// MAIN LOOP
// ======================================================

void loop()
{
  // Stop gameplay once the player has won.
  // The scene and victory message will remain visible.

  if (!reachedGoal)
  {
    // Start each frame with no recorded death.

    playerDiedThisFrame = false;


    // --------------------------------------------------
    // MOVE LEFT / RIGHT
    // --------------------------------------------------

    int speed = moveSpeed + (dashActive ? dashBonusSpeed : 0);

    int xValue = analogRead(JOY_X);

    if (xValue < 400)
    {
      playerX -= speed;
    }

    if (xValue > 600)
    {
      playerX += speed;
    }

    playerX = constrain(
      playerX,
      0,
      LEVEL_WIDTH - playerWidth
    );


    // --------------------------------------------------
    // JUMP
    // --------------------------------------------------

    if (buttonPressedEdge())
    {
      if (onGround)
      {
        velocityY = jumpSpeed;
      }
    }


    // --------------------------------------------------
    // DASH
    // --------------------------------------------------

    handleDash();


    // --------------------------------------------------
    // MOVING PLATFORMS
    // --------------------------------------------------

    moveMovingPlatforms();


    // --------------------------------------------------
    // GRAVITY
    // --------------------------------------------------

    playerPrevY = playerY;

    onGround = false;

    velocityY += gravity;
    playerY += velocityY;


    // --------------------------------------------------
    // PLATFORM COLLISION
    // --------------------------------------------------

    resolveSolidPlatforms();
    resolveMovingPlatformCollisions();
    updateBreakablePlatforms();


    // --------------------------------------------------
    // ENEMIES
    // --------------------------------------------------

    updateEnemies();
    checkEnemyCollisions();


    // --------------------------------------------------
    // SPIKES
    // --------------------------------------------------
    //
    // Skip this check if an enemy has already caused
    // a death during this frame.
    // --------------------------------------------------

    if (!playerDiedThisFrame)
    {
      checkSpikeCollisions();
    }


    // --------------------------------------------------
    // FALL DEATH
    // --------------------------------------------------
    //
    // Skip this check if something else has already
    // caused a death during this frame.
    // --------------------------------------------------

    if (!playerDiedThisFrame)
    {
      checkFallDeath();
    }


    // --------------------------------------------------
    // STARS AND GOAL
    // --------------------------------------------------
    //
    // Do not collect stars or complete the level on
    // the same frame as a death or full reset.
    // --------------------------------------------------

    if (!playerDiedThisFrame)
    {
      checkStarCollision();
      updateGoal();
    }
  }


  // ----------------------------------------------------
  // UPDATE INVINCIBILITY
  // ----------------------------------------------------

  updateInvincibility();


  // ----------------------------------------------------
  // UPDATE CAMERA
  // ----------------------------------------------------

  updateCamera();


  // ----------------------------------------------------
  // DRAW EVERYTHING
  // ----------------------------------------------------

  display.clearDisplay();

  drawSolidPlatforms();
  drawMovingPlatforms();
  drawBreakablePlatforms();

  drawSpikes();
  drawStars();
  drawEnemies();

  drawFlag();
  drawPlayer();

  drawGoalMessage();
  drawHUD();
  drawDashBar();

  display.display();

  delay(10);
}
